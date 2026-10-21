/*
 * A063286  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          13 ).
 * https://oeis.org/A063286
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n > 13. Initial values:
 *            a(2) = -1, a(3) = 2, a(4) = 8, a(5) = 15, a(6) = 22, a(7) = 29,
 *            a(8) = 36, a(9) = 43, a(10) = 50, a(11) = 57, a(12) = 64, a(13)
 *            = 69
 * Range:     i8 n <= 21, i16 n <= 54, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values.  cache[n] holds a(n). */
    static seq_t *cache = NULL;
    static idx_t cache_capacity = 0;   /* size of allocated array */
    static idx_t max_cached     = 1;   /* highest index that has been computed */

    /* Return initial values directly when n ≤ 13.  We need only indices 2–13. */
    if (n <= 13) {
        /* The following array is indexed by n (0‑based).  Positions 0,1 are unused. */
        static const seq_t init[14] = {
            0, 0,           /* 0, 1 – not used */
            -1,              /* 2 */
            2,               /* 3 */
            8,               /* 4 */
            15,              /* 5 */
            22,              /* 6 */
            29,              /* 7 */
            36,              /* 8 */
            43,              /* 9 */
            50,              /*10 */
            57,              /*11 */
            64,              /*12 */
            69               /*13 */
        };
        return init[n];
    }

    /* For n > 13 we compute via recurrence, using memoization. */
    /* Ensure the cache is large enough. */
    if (n + 1 > cache_capacity) {
        /* Grow the cache to accommodate index n. */
        idx_t new_capacity = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_capacity * sizeof(seq_t));
        /* If realloc fails the program is in an unrecoverable state. */
        if (!tmp) return 0;  /* defensive: return 0 on allocation failure */
        cache = tmp;
        cache_capacity = new_capacity;
    }

    /* If we do not have values up to ``n`` previously, compute them. */
    if (n > max_cached) {
        /* Initialize base cases in the cache the first time. */
        if (max_cached < 14) {
            /* Fill indices 2..13 with their known values. */
            cache[2] = -1;
            cache[3] =  2;
            cache[4] =  8;
            cache[5] = 15;
            cache[6] = 22;
            cache[7] = 29;
            cache[8] = 36;
            cache[9] = 43;
            cache[10] = 50;
            cache[11] = 57;
            cache[12] = 64;
            cache[13] = 69;
            max_cached = 13;
        }
        /* Compute values from max_cached+1 up to n. */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            cache[i] = cache[i-4] + cache[i-6] - cache[i-10];
        }
        max_cached = n;
    }

    return cache[n];
}
