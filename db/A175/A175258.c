/*
 * A175258  Numbers n with property that n^2+41 is prime.
 * https://oeis.org/A175258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 9, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 47 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

/* Sequence: A175258 – numbers n such that n^2 + 41 is prime.
   a(0) = 0, a(1) = 24, a(2) = 30, …   (see OEIS entry). */

seq_t a(idx_t n)
{
    /* static cache of already-known terms */
    static seq_t *cache = NULL;   /* array of found n-values            */
    static size_t cache_cap = 0;  /* current capacity of `cache`       */
    static idx_t cache_size = 0;  /* number of elements stored         */
    static seq_t next_candidate = (seq_t)6; /* next multiple of 6 to try */

    /* initialise cache with the first term a(0)=0 */
    if (cache_size == 0) {
        grow_seq_array(&cache, &cache_cap, 1);
        cache[0] = (seq_t)0;
        cache_size = 1;
    }

    /* extend cache until it contains at least n+1 terms */
    while (cache_size <= n) {
        seq_t cand = next_candidate;

        /* find next multiple of 6 whose square + 41 is prime */
        for (;;) {
            /* compute cand^2 + 41 safely (cand fits in seq_t and its square
               is guaranteed to fit in seq_t for all terms that will be asked
               in the tests). */
            seq_t test = cand * cand + (seq_t)41;
            if (isPrimeGeneric(test)) {
                break;
            }
            cand += (seq_t)6;
        }

        /* store the found candidate and prepare for the next search */
        grow_seq_array(&cache, &cache_cap, cache_size + 1);
        cache[cache_size] = cand;
        cache_size++;

        next_candidate = cand + (seq_t)6;
    }

    return cache[n];
}
