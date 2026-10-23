/*
 * A033765  Product t2(q^d); d | 6, where t2 = theta2(q)/(2*q^(1/4)).
 * https://oeis.org/A033765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = number of positive odd integer solutions of a^2 + 2*b^2 +
 *            3*c^2 + 6*d^2 = 8*n + 12 (a,b,c,d > 0, odd)
 * Range:     u8 n <= 290, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a static cache that is grown when needed.   */
/* It enumerates all odd (a,b,c,d) up to the maximal value required
   and records the counts for each n.                         */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_max = 0;   /* largest n whose value was cached */

/* Helper to grow the cache to at least size n+1 */
static void
grow_cache(idx_t n)
{
    if (cache != NULL && n <= cache_max)
        return;                /* nothing to do */

    idx_t newmax = n;
    idx_t maxSum = 8 * newmax + 12;   /* maximum V = a^2+2b^2+3c^2+6d^2 */

    /* Allocate a zeroed array for the counts */
    seq_t *newcache = (seq_t *)calloc((size_t)(newmax + 1), sizeof(seq_t));
    if (newcache == NULL) abort();  /* out of memory */

    /* Enumerate all odd quadruples */
    for (idx_t d = 1; ; d += 2) {
        idx_t d2 = 6 * d * d;
        if (d2 > maxSum) break;
        for (idx_t c = 1; ; c += 2) {
            idx_t d2c2 = d2 + 3 * c * c;
            if (d2c2 > maxSum) break;
            for (idx_t b = 1; ; b += 2) {
                idx_t tmp = d2c2 + 2 * b * b;
                if (tmp > maxSum) break;
                for (idx_t a = 1; ; a += 2) {
                    idx_t V = tmp + a * a;
                    if (V > maxSum) break;
                    if (V >= 12 && ((V - 12) & 7) == 0) {
                        idx_t k = (V - 12) >> 3;   /* (V-12)/8 */
                        if (k <= newmax)
                            newcache[k] += 1;
                    }
                }
            }
        }
    }

    /* Replace old cache */
    if (cache != NULL)
        free(cache);
    cache = newcache;
    cache_max = newmax;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (cache == NULL || n > cache_max)
        grow_cache(n);
    return cache[n];
}
