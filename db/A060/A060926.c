/*
 * A060926  Row sums of triangle A060923 (even part of bisection of Lucas
 *          triangle).
 * https://oeis.org/A060926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1+4×x²)/(1-5×x-4×x³) a(0) = 1, a(1) = 5, a(2) = 29 a(n)
 *            = 5·a(n-1) + 4·a(n-3) for n ≥ 3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t, NULL */
#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* Cached values up to last computed index */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements stored */

    /* If we already have the value, return it immediately. */
    if (n < cache_len)
        return cache[(size_t)n];

    /* Compute value without caching if n is too large for an array. */
    if (n >= SIZE_MAX - 1) {
        /* Compute iteratively using only the last three values. */
        seq_t a0 = (seq_t)1;
        if (n == 0) return a0;
        seq_t a1 = (seq_t)5;
        if (n == 1) return a1;
        seq_t a2 = (seq_t)29;
        if (n == 2) return a2;
        seq_t a_nm3 = a0, a_nm2 = a1, a_nm1 = a2, a_n = 0;
        for (size_t i = 3; i <= (size_t)n; ++i) {
            a_n = (seq_t)5 * a_nm1 + (seq_t)4 * a_nm3;
            a_nm3 = a_nm2;
            a_nm2 = a_nm1;
            a_nm1 = a_n;
        }
        return a_n;
    }

    /* Ensure the cache array can hold indices up to n. */
    size_t new_len = (size_t)n + 1;
    if (new_len > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure  */
        cache     = tmp;
        cache_len = new_len;

        /* Initialize base cases in the expanded region. */
        if (cache_len >= 1) cache[0] = (seq_t)1;
        if (cache_len >= 2) cache[1] = (seq_t)5;
        if (cache_len >= 3) cache[2] = (seq_t)29;

        /* Compute subsequent terms using the recurrence. */
        for (size_t i = 3; i < cache_len; ++i) {
            cache[i] = (seq_t)5 * cache[i-1] + (seq_t)4 * cache[i-3];
        }
    }

    return cache[(size_t)n];
}
