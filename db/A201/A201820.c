/*
 * A201820  Numbers k such that 90*k + 23 is prime.
 * https://oeis.org/A201820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 445, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9369 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A201820: a(n) = (p_n - 23) / 90 where p_n is the n‑th prime
   congruent to 23 (mod 90).  a(n) is the corresponding k such that
   90*k + 23 is prime. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

static seq_t *cache = NULL;      /* cached k values */
static size_t cache_len = 0;     /* number of cached terms */
static size_t cache_cap = 0;     /* current allocation capacity */
static idx_t cur_candidate = 0;  /* next k to test */

/* Return the n-th value a(n), 1‑based. */
seq_t a(idx_t n)
{
    /* Ensure we have computed at least n terms. */
    while (cache_len < n) {
        /* Number 90*k + 23 to test for primality. */
        seq_t candidate = (seq_t)cur_candidate * 90 + 23;

        if (isPrimeGeneric(candidate)) {
            if (cache_len == cache_cap) {
                /* Grow the cache to accommodate another entry. */
                grow_seq(&cache, &cache_cap, cache_len + 1);
            }
            cache[cache_len++] = (seq_t)cur_candidate;
        }
        /* Move to next k. */
        cur_candidate++;
    }
    return cache[n - 1];
}
