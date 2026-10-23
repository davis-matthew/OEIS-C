/*
 * A157257  Positive numbers y such that y^2 is of the form x^2+(x+41)^2 with
 *          integer x.
 * https://oeis.org/A157257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) for n > 6; a(1)=29, a(2)=41, a(3)=85,
 *            a(4)=89, a(5)=205, a(6)=481
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 34, u64 n <= 71, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;   /* cache[0] unused */
    static idx_t cache_size  = 0;      /* allocated size (indices 0..cache_size-1) */
    static idx_t max_computed= 0;      /* largest index whose value is known */

    /* Ensure the cache is large enough for the first six terms and
       initialise them once. */
    if (max_computed < 6) {
        if (!cache) {
            cache = (seq_t *)malloc((6 + 1) * sizeof(seq_t));   /* indices 0..6 */
            if (!cache) exit(EXIT_FAILURE);
            cache_size = 6;
        }
        cache[1] = 29;   /* a(1) */
        cache[2] = 41;   /* a(2) */
        cache[3] = 85;   /* a(3) */
        cache[4] = 89;   /* a(4) */
        cache[5] = 205;  /* a(5) */
        cache[6] = 481;  /* a(6) */
        max_computed = 6;
    }

    /* Grow the cache if n is beyond the current allocation. */
    if (n > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache      = tmp;
        cache_size = n;
    }

    /* For n <= 6 the value is already stored. */
    if (n <= 6)
        return cache[n];

    /* Compute the sequence up to n using the recurrence. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = (seq_t)6 * cache[i - 3] - cache[i - 6];
    }
    max_computed = n;

    return cache[n];
}
