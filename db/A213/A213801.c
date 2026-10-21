/*
 * A213801  Number of 3 X 3 0..n symmetric arrays with all rows summing to
 *          floor(n*3/2).
 * https://oeis.org/A213801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + 2*a(n-4) - 2*a(n-5) + 2*a(n-7) -
 *            a(n-8); Initial values: a(1)=4, a(2)=13, a(3)=29, a(4)=57,
 *            a(5)=96, a(6)=153, a(7)=226, a(8)=323
 * Range:     u8 n <= 7, u16 n <= 52, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage function required by the build system */
seq_t a(idx_t n)
{
    /* Return 0 for dangling indices; behaviour below offset is unspecified. */
    if (n < 1) return 0;

    /* Base values for n = 1 .. 8 */
    static const seq_t base[9] = {0, 4, 13, 29, 57, 96, 153, 226, 323};

    if (n < 9) return base[(size_t)n];

    static seq_t *cache = NULL;   /* dynamic array storing computed terms */
    static idx_t cache_max = 0;   /* highest index currently stored */

    /* Allocate cache lazily */
    if (!cache) {
        /* Start with enough room for indices 0 .. 8 */
        cache_max = 8;
        cache = (seq_t *)malloc((cache_max + 1) * sizeof(seq_t));
        if (!cache) abort(); /* fatal if memory unavailable */
        for (size_t i = 1; i <= 8; ++i) cache[i] = base[i];
    }

    /* Grow cache if needed and compute missing terms */
    if (n > cache_max) {
        /* Double the capacity, rounded up to n+1 */
        idx_t new_cap = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* Compute terms from cache_max+1 up to n */
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            if (i < 9) {
                cache[i] = base[(size_t)i];
            } else {
                /* Recurrence uses only additions/subtractions
                   that never overflow for unsigned types up to the
                   sequence values known to fit in the target width. */
                cache[i] =
                    2*cache[i-1] - 2*cache[i-3] + 2*cache[i-4] - 2*cache[i-5]
                    + 2*cache[i-7] - cache[i-8];
            }
        }
        cache_max = n;
    }

    return cache[(size_t)n];
}
