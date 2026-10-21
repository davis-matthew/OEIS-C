/*
 * A228733  Number of arrays of the median of three adjacent elements of some
 *          length n+2 0..1 array.
 * https://oeis.org/A228733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-3) + a(n-5) (for n ≥ 6) Initial values:
 *            a(1)=2, a(2)=4, a(3)=8, a(4)=16, a(5)=28
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 37, u64 n <= 75, u128 n <= 152,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* cache[i-1] holds a(i)      */
    static idx_t cached      = 0;      /* largest index computed     */
    static const seq_t base_vals[5] = {2, 4, 8, 16, 28};

    /* Handle the first five terms directly. */
    if (n <= 5)
        return base_vals[n - 1];

    /* Grow the cache if necessary and compute missing terms. */
    if (cached < n) {
        if (cached == 0) {
            cache = (seq_t*)malloc((size_t)n * sizeof(seq_t));
            for (idx_t i = 0; i < 5; ++i)  /* fill the known values   */
                cache[i] = base_vals[i];
            cached = 5;
        } else {
            cache = (seq_t*)realloc(cache, (size_t)n * sizeof(seq_t));
        }

        for (idx_t i = cached + 1; i <= n; ++i) {
            /* a(i) = 2*a(i-1) - a(i-3) + a(i-5) (indices are 1‑based) */
            cache[i - 1] = 2 * cache[i - 2]
                         - cache[i - 4]
                         + cache[i - 6];
        }
        cached = n;
    }

    return cache[n - 1];
}
