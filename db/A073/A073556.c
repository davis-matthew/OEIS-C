/*
 * A073556  Number of Fibonacci numbers F(k), k <= 10^n, which end in 9.
 * https://oeis.org/A073556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = 11*a(n-1) - 10*a(n-2) for n > 3, with a(1) = 0, a(2) =
 *            13, a(3) = 133. The sequence counts Fibonacci numbers ≤10^n that
 *            end with 9
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Ensure n is at least 1 (undefined otherwise) */
    if (n < 1) return 0;

    /* Basic known values */
    if (n == 1) return 0;
    if (n == 2) return 13;
    if (n == 3) return 133;

    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Grow the cache to at least index 3 */
    if (cache == NULL) {
        cache_size = 4;                /* indices 0..3 ; 0 unused */
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = 0;   /* unused */
        cache[1] = 0;
        cache[2] = 13;
        cache[3] = 133;
    }

    /* Extend cache if needed */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        for (idx_t i = cache_size; i < new_size; ++i)
            cache[i] = 0;                /* initialise */
        cache_size = new_size;
    }

    /* Compute missing values using the recurrence */
    for (idx_t i = 4; i <= n; ++i) {
        if (cache[i] == 0) {           /* compute only if not already done */
            /* Use seq_t arithmetic; intermediate overflow is
               benign for tests that fit in seq_t. */
            cache[i] = 11 * cache[i-1] - 10 * cache[i-2];
        }
    }

    return cache[n];
}
