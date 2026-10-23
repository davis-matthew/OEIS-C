/*
 * A013587  Number of rational plane curves of degree d passing through 3d-1
 *          general points.
 * https://oeis.org/A013587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(d) = Σ_{i+j=d} a(i)*a(j) * ( i^2*j^2*binomial(3d-4,3i-2) –
 *            i^3*j *binomial(3d-4,3i-1) ) with a(1)=1, a(2)=1. The offset is
 *            1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"   /* binomial(idx_t a, idx_t b) -> seq_t */

seq_t a(idx_t n)
{
    /* Lazy memoisation */
    static seq_t *cache     = NULL;   /* cache[1..max_computed] */
    static idx_t  cache_sz  = 0;      /* allocated size ( >= n+1 ) */
    static idx_t  max_comp  = 0;      /* highest d already computed */

    if (n < 1) return 0;              /* undefined for n<=0 */

    /* Grow cache if needed */
    if (cache_sz < n + 1) {
        seq_t *newc = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!newc) return 0;          /* out of memory, return 0 */
        memset(newc, 0, (n + 1) * sizeof(seq_t));
        if (cache) {
            memcpy(newc, cache, cache_sz * sizeof(seq_t));
            free(cache);
        }
        cache     = newc;
        cache_sz  = n + 1;
    }

    /* Initialise base values once */
    if (max_comp == 0) {
        cache[1] = 1;
        if (n >= 2) cache[2] = 1;
        max_comp = (n >= 2) ? 2 : 1;
    }

    /* Compute missing values up to n */
    if (max_comp < n) {
        for (idx_t d = max_comp + 1; d <= n; ++d) {
            seq_t sum = 0;
            for (idx_t i = 1; i < d; ++i) {
                idx_t j = d - i;
                seq_t a_i = cache[i];
                seq_t a_j = cache[j];

                seq_t bin1 = binomial(3 * d - 4, 3 * i - 2);
                seq_t bin2 = binomial(3 * d - 4, 3 * i - 1);

                seq_t i2 = (seq_t)i * (seq_t)i;
                seq_t j2 = (seq_t)j * (seq_t)j;
                seq_t i3 = i2 * (seq_t)i;

                seq_t term1 = i2 * j2 * bin1;
                seq_t term2 = i3 * (seq_t)j * bin2;
                seq_t diff  = term1 - term2;   /* non‑negative by definition */

                sum += a_i * a_j * diff;
            }
            cache[d] = sum;
        }
        max_comp = n;
    }

    return cache[n];
}
