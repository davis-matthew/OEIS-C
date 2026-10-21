/*
 * A240638  Number of n X 3 0..3 arrays with no element equal to one or three
 *          horizontal or vertical neighbors, with new values 0..3 introduced
 *          in row major order.
 * https://oeis.org/A240638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) + 54*a(n-2) + 82*a(n-3) - 14*a(n-4) * -
 *            54*a(n-5) - 81*a(n-6) for n ≥ 7 * Initial values: a(1)=2,
 *            a(2)=27, a(3)=462, a(4)=7993, * a(5)=138882, a(6)=2413198
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* sequence starts at n=1 */

    /* static cache for memoisation */
    static seq_t *cache = NULL;      /* array of terms (1‑based index shifted to 0) */
    static idx_t  cached_n = 0;      /* largest n for which cache is filled */

    /* grow cache if necessary */
    if (n > cached_n) {
        /* reallocate to hold at least n terms */
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!new_cache)
            return 0;          /* memory allocation failed – unlikely in tests */
        cache = new_cache;

        /* initialise first six terms (if they are not already present) */
        const seq_t init[6] = {2, 27, 462, 7993, 138882, 2413198};
        idx_t limit = n < 6 ? n : 6;
        for (idx_t i = 1; i <= limit; ++i) {
            cache[i - 1] = init[i - 1];
        }

        /* fill the rest of the cache up to n using the recurrence */
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            if (i <= 6) {
                /* already initialised above */
                continue;
            }
            /* recurrence: a(i) = 14*a(i-1) + 54*a(i-2) + 82*a(i-3)
                           - 14*a(i-4) - 54*a(i-5) - 81*a(i-6) */
            seq_t term =
                14 * cache[i - 2]                     /* a(i-1) */
              + 54 * cache[i - 3]                     /* a(i-2) */
              + 82 * cache[i - 4]                     /* a(i-3) */
              - 14 * cache[i - 5]                     /* a(i-4) */
              - 54 * cache[i - 6]                     /* a(i-5) */
              - 81 * cache[i - 7];                    /* a(i-6) */
            cache[i - 1] = term;
        }
        cached_n = n;      /* update the highest computed index */
    }
    return cache[n - 1];
}
