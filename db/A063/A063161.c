/*
 * A063161  Dimension of the space of weight 2n cusp forms for Gamma_0( 93 ).
 * https://oeis.org/A063161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) for n > 5 Initial values:
 *            a(1)=9, a(2)=30, a(3)=52, a(4)=72, a(5)=94
 * Range:     u8 n <= 12, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle below offset – return 0 */
    if (n < 1)
        return (seq_t)0;

    /* cache the sequence up to the largest n requested */
    static seq_t *cache = NULL;       /* holds a[0]..a[max] (a[0] unused) */
    static size_t cache_size = 0;     /* number of elements allocated */

    if (n > 5) {
        /* need to expand cache if necessary */
        if (cache == NULL) {
            /* initial allocation up to n=5 */
            size_t init = 6;               /* indices 0..5 */
            cache = (seq_t *)malloc(init * sizeof(seq_t));
            if (!cache) return (seq_t)0;   /* allocation failure – give up */
            cache[0] = (seq_t)0;           /* unused */
            cache[1] = (seq_t)9;
            cache[2] = (seq_t)30;
            cache[3] = (seq_t)52;
            cache[4] = (seq_t)72;
            cache[5] = (seq_t)94;
            cache_size = init;
        }

        /* enlarge cache as needed */
        if ((size_t)n + 1 > cache_size) {
            size_t new_size = (size_t)n + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (!tmp) return cache[n]; /* realloc failed – return what we have */
            cache = tmp;
            /* fill new entries using the recurrence */
            for (size_t i = cache_size; i <= (size_t)n; ++i) {
                cache[i] = cache[i-1] + cache[i-3] - cache[i-4];
            }
            cache_size = new_size;
        }

        return cache[n];
    }

    /* n <= 5 – return the hard‑coded initial values */
    switch ((int)n) {
        case 1: return (seq_t)9;
        case 2: return (seq_t)30;
        case 3: return (seq_t)52;
        case 4: return (seq_t)72;
        case 5: return (seq_t)94;
        default: return (seq_t)0;   /* should not happen */
    }
}
