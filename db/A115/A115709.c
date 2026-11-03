/*
 * A115709  Pentagonal numbers (A000326) whose digit reversal is a semiprime
 *          (A001358).
 * https://oeis.org/A115709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 56, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 983 (2026-11-03)
 * Helpers:   is_prime_u64, reverse_digits_idx, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A115709: pentagonal numbers P(k)=k*(3k-1)/2 whose digit reversal is a semiprime. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/reverse_digits_idx.h"
#include "../utils/smallest_prime_factor.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *cache = NULL;          /* cached valid pentagonal numbers */
static size_t cache_len = 0;         /* number of cached entries          */
static size_t cache_cap = 0;         /* current allocated capacity        */
static idx_t  last_k   = 0;          /* last k processed in the sieve    */

/* grow the cache to at least new_cap entries */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    seq_t *p = seq_realloc(cache, cache_cap, new_cap);
    if (!p) abort();                /* out of memory */
    cache   = p;
    cache_cap = new_cap;
}

/* check whether x is a semiprime (product of exactly two primes) */
static bool is_semiprime_uint64(uint64_t x)
{
    if (x < 4) return false;               /* 4 = 2*2 is the smallest semiprime */
    uint64_t spf = smallest_prime_factor(x);
    if (spf <= 1) return false;
    uint64_t q = x / spf;
    return is_prime_big((seq_t)q);
}

/* the required sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1 – safety return */

    /* generate terms until we have at least n cached */
    while (cache_len < (size_t)n)
    {
        last_k++;                                   /* next k */
        seq_t k = (seq_t)last_k;
        seq_t pent = k * (3 * k - 1) / 2;            /* pentagonal number */

        uint64_t rev_val = (uint64_t)reverse_digits_idx((idx_t)pent);
        if (is_semiprime_uint64(rev_val))
        {
            if (cache_len == cache_cap) grow_cache(cache_cap ? cache_cap * 2 : 16);
            cache[cache_len++] = pent;
        }
    }

    return cache[(size_t)(n - 1)];                 /* 1‑based indexing */
}
