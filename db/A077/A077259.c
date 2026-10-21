/*
 * A077259  First member of the Diophantine pair (m,k) that satisfies 5*(m^2 +
 *          m) = k^2 + k; a(n) = m.
 * https://oeis.org/A077259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0, a(1)=2, a(2)=6, a(3)=44, a(4)=116 For n≥5:
 *            a(n)=a(n-1)+18*a(n-2)-18*a(n-3)-a(n-4)+a(n-5) (implemented via
 *            positive differences to avoid unsigned underflow) This
 *            recurrence generates the terms of OEIS A077259
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* base values for n = 0..4 */
    static seq_t base[5] = { (seq_t)0, (seq_t)2, (seq_t)6,
                             (seq_t)44, (seq_t)116 };

    /* cache to store computed values up to the largest request */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* handle small indices directly */
    if (n < 5) {
        return base[n];
    }

    /* initialise cache on first use */
    if (cache == NULL) {
        cache_size = 5;                     /* need indices 0..4 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failed, return 0 */
        cache[0] = base[0];
        cache[1] = base[1];
        cache[2] = base[2];
        cache[3] = base[3];
        cache[4] = base[4];
    }

    /* enlarge cache if needed and compute new entries */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;         /* allocation failed */
        cache = tmp;

        /* compute from first missing index up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* diff1 = a[i-2] - a[i-3]  (positive because the sequence is increasing) */
            seq_t diff1 = cache[i-2] - cache[i-3];
            seq_t part1 = cache[i-1] + (seq_t)18 * diff1;

            /* diff2 = a[i-4] - a[i-5]  (positive as well) */
            seq_t diff2 = cache[i-4] - cache[i-5];

            cache[i] = part1 - diff2;       /* part1 > diff2 holds for this sequence */
        }
        cache_size = new_size;
    }

    return cache[n];
}
