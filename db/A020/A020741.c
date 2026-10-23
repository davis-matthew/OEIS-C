/*
 * A020741  Pisot sequence T(6,10), a(n) = floor(a(n-1)^2/a(n-2)).
 * https://oeis.org/A020741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(a(n-1)^2 / a(n-2)), with a(0)=6, a(1)=10
 * Range:     u8 n <= 8, u16 n <= 21, u32 n <= 47, u64 n <= 100, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine caches previously computed terms so that calls with
 * arbitrary order of indices run in O(k) where k is the maximum
 * index requested.
 * It uses an intermediate 128‑bit product when seq_t is 64‑bit
 * or smaller, thus keeping rounding correct even when the
 * final result still fits in seq_t. */

#include <stdlib.h>   /* malloc, realloc, size_t */

seq_t a(idx_t n)
{
    /* Static cache for computed sequence values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* number of terms computed */

    /* Ensure there is room for at least n+1 terms */
    if (n + 1 > cache_cap) {
        size_t newcap = cache_cap > 0 ? cache_cap * 2 : 2;
        if (newcap < (size_t)n + 1) newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp && newcap) return 0;   /* allocation failed; undefined behaviour */
        cache = tmp;
        cache_cap = newcap;
    }

    /* Initialise the first two terms if this is the first call */
    if (cache_len == 0) {
        cache[0] = (seq_t)6;
        cache[1] = (seq_t)10;
        cache_len = 2;
    }

    /* Compute any missing terms up to n */
    while (cache_len <= n) {
        seq_t prev1 = cache[cache_len - 1];
        seq_t prev2 = cache[cache_len - 2];
        seq_t next;

        /* Use a wider intermediate type if seq_t is small enough to avoid overflow */
        if (sizeof(seq_t) <= 8) {
            unsigned __int128 prod = (unsigned __int128)prev1 * (unsigned __int128)prev1;
            next = (seq_t)(prod / prev2);
        } else {
            /* For larger integer or arbitrary‑precision types, normal arithmetic is exact */
            next = (prev1 * prev1) / prev2;
        }

        cache[cache_len++] = next;
    }

    return cache[n];
}
