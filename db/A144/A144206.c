/*
 * A144206  Numbers A141427(k) such that the three numbers A141427(k) -/+ 3
 *          and A141427(k) + 1 are all prime.
 * https://oeis.org/A144206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th integer a such that a - 3, a + 1, a + 3 are all
 *            primes. Computation is done by sieving primes up to a growing
 *            upper bound until at least n terms have been found
 * Range:     u8 n <= 8, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* External integer types supplied by the build. */
extern seq_t a(idx_t n);

/* -----  Sieve of Eratosthenes (odd‑only)  ----- */
static uint64_t *bits = NULL;          /* bitset representing odd composites */
static size_t  bits_len = 0;           /* number of odd numbers <= current limit */

/* Dynamic array of sequence terms */
static seq_t  *terms   = NULL;
static size_t   term_len = 0;
static size_t   term_cap = 0;

/* Helper to mark an odd number as composite */
static inline void set_composite(uint64_t num)
{
    uint64_t idx = (num - 3) >> 1;        /* index for odd number >=3 */
    bits[idx >> 6] |= (1ULL << (idx & 63));
}

/* Helper to test whether an odd number >=3 is composite */
static inline bool is_composite(uint64_t num)
{
    uint64_t idx = (num - 3) >> 1;
    return (bits[idx >> 6] >> (idx & 63)) & 1ULL;
}

/* Rebuild the sieve and rebuild the list of a(n) terms up to `limit`. */
static void rebuild(idx_t limit)
{
    /* ---------- allocate and zero the bitset ---------- */
    size_t bit_count = (limit >= 3) ? (((size_t)(limit - 3) >> 1) + 1) : 0;
    size_t array_len = (bit_count + 63) >> 6;          /* number of 64‑bit words */

    free(bits);
    bits = (uint64_t *)calloc(array_len, sizeof(uint64_t));
    bits_len = bit_count;

    /* ---------- sieve primes up to sqrt(limit) ---------- */
    uint64_t sqrt_limit = (uint64_t)sqrt((double)limit);
    for (uint64_t i = 3; i <= sqrt_limit; i += 2) {
        if (is_composite(i)) continue;
        uint64_t step = i << 1;                    /* skip even multiples */
        for (uint64_t j = i * i; j <= limit; j += step)
            set_composite(j);
    }

    /* ---------- rebuild sequence terms ---------- */
    free(terms);
    terms     = NULL;
    term_len  = 0;
    term_cap  = 0;

    for (uint64_t n = 3; n + 6 <= limit; n += 2) {   /* n is an odd candidate */
        if (is_composite(n))          continue;
        if (is_composite(n + 4))      continue;
        if (is_composite(n + 6))      continue;

        /* Found a triple (n, n+4, n+6) meaning a = n+3 */
        if (term_len == term_cap) {
            term_cap = term_cap ? term_cap * 2 : 16;
            terms = (seq_t *)realloc(terms, term_cap * sizeof(seq_t));
        }
        terms[term_len++] = (seq_t)(n + 3);
    }
}

/* -----  Public function  ----- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe guard */

    static bool initialized = false;
    static idx_t cur_limit  = 0;

    if (!initialized) {
        cur_limit = 20000000U;     /* start with a modest limit */
        rebuild(cur_limit);
        initialized = true;
    }

    while ((idx_t)term_len < n) {
        cur_limit *= 2;
        rebuild(cur_limit);
    }

    return terms[n - 1];
}
