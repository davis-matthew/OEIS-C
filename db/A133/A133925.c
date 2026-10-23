/*
 * A133925  Number of compositions of n into parts of size 2 and 3 with no
 *          three consecutive 2s and no two consecutive 3s.
 * https://oeis.org/A133925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = -a(n-1) + a(n-3) + a(n-4) + a(n-5) with initial values:
 *            a(1)=0, a(2)=1, a(3)=1, a(4)=1, a(5)=2, a(6)=0
 * Range:     u8 n <= 49, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle non‑positive indices (unused by callers) */
    if (n == 0) return (seq_t)0;

    /* static cache for memoisation */
    static seq_t *cache = NULL;     /* holds a(0)..a(max_computed) */
    static size_t cache_size = 0;   /* allocated size */
    static idx_t  max_computed = 0; /* highest n that has been computed */

    /* ensure cache large enough for requested n */
    if (cache == NULL || cache_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;   /* indices 0..n */
        if (new_size < 7) new_size = 7;    /* at least up to a(6) */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;         /* allocation failure – return 0 */
        cache = tmp;
        cache_size = new_size;

        /* initialise base values if not yet done */
        if (max_computed < 6) {
            cache[0] = (seq_t)0;          /* unused */
            cache[1] = (seq_t)0;          /* a(1) */
            cache[2] = (seq_t)1;          /* a(2) */
            cache[3] = (seq_t)1;          /* a(3) */
            cache[4] = (seq_t)1;          /* a(4) */
            cache[5] = (seq_t)2;          /* a(5) */
            cache[6] = (seq_t)0;          /* a(6) */
            max_computed = 6;
        }
    }

    /* compute any missing values up to n */
    if (max_computed < (idx_t)n) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* recurrence: a(i) = -a(i-1) + a(i-3) + a(i-4) + a(i-5) */
            seq_t sum = cache[(size_t)(i - 3)] + cache[(size_t)(i - 4)] +
                        cache[(size_t)(i - 5)];
            cache[(size_t)i] = sum - cache[(size_t)(i - 1)];
        }
        max_computed = n;
    }

    return cache[(size_t)n];
}
