/*
 * A051373  a(n+1) = a(n) + sum of digits of (a(n)^3).
 * https://oeis.org/A051373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 48, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence definition of OEIS A051373.
   a(1) = 1
   a(n+1) = a(n) + sum_of_digits(a(n)^3)   for n ≥ 1
   The function memoizes the sequence in a dynamic array so that calls
   with arbitrary order of n are supported. */

#include <stdlib.h>          /* for malloc, realloc */

#include "../utils/digit_sum.h"   /* seq_t digit_sum(seq_t v); */

/* Static cache for computed terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms currently stored  (≥0) */
static size_t cache_cap = 0;   /* allocated capacity */

/* Grow the cache array so that it can hold at least min_cap elements. */
static void grow_cache(size_t min_cap)
{
    if (min_cap <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < min_cap)
        new_cap <<= 1;                       /* double until large enough */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term (1‑based).  n ≥ 1. */
seq_t a(idx_t n)
{
    /* Initialise cache with the first term, if not already done. */
    if (cache_len == 0) {
        grow_cache(1);
        cache[0] = (seq_t)1;   /* a(1) */
        cache_len = 1;
    }

    /* Compute missing terms up to n. */
    while (cache_len < (size_t)n) {
        seq_t prev = cache[cache_len - 1];

        /* Compute a(n)^3 in the natural arithmetic of seq_t. */
        seq_t cube = prev * prev * prev;

        /* Sum of its decimal digits. */
        seq_t inc = digit_sum(cube);

        /* Next term. */
        seq_t next = prev + inc;

        grow_cache(cache_len + 1);
        cache[cache_len] = next;
        cache_len++;
    }

    return cache[n - 1];
}
