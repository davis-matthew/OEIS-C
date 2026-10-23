/*
 * A033727  Product theta3(q^d); d | 15.
 * https://oeis.org/A033727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of q^n in Π_{d|15} θ3(q^d), where θ3(q)
 *            = 1 + 2 Σ_{m≥1} q^{m²}. Coefficients are given by a(n) =
 *            Σ_{m1,m2,m3,m4≥0 ; n = m1² + 3m2² + 5m3² + 15m4²} 2^{# of
 *            non‑zero mi}. The implementation builds the product by
 *            successive convolutions of the sparse series 1 + 2 Σ q^{d·m²}
 *            f...
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* Cache of computed coefficients up to the largest requested n. */
    static seq_t *cache = NULL;     /* sequence array                          */
    static idx_t cache_size = 0;    /* number of entries in cache              */

    /* Allocate/extend cache as needed. */
    if (n + 1 > cache_size) {
        /* Free previous cache. */
        if (cache) free(cache);

        /* New cache size. */
        cache_size = n + 1;

        /* Allocate and initialise: start with P(q)=1. */
        seq_t *cur = (seq_t*)malloc(sizeof(seq_t) * cache_size);
        memset(cur, 0, sizeof(seq_t) * cache_size);
        cur[0] = (seq_t)1;

        /* Divisors of 15. */
        const idx_t divisors[4] = {1, 3, 5, 15};

        /* For each divisor d, convolve the current polynomial
           with the sparse series   1 + 2 Σ_{m≥1} q^{d·m²}.  */
        for (int di = 0; di < 4; ++di) {
            idx_t d = divisors[di];

            /* Temporary array for the new polynomial. */
            seq_t *tmp = (seq_t*)malloc(sizeof(seq_t) * cache_size);
            memset(tmp, 0, sizeof(seq_t) * cache_size);

            /* Convolution. */
            for (idx_t i = 0; i < cache_size; ++i) {
                seq_t vi = cur[i];
                if (!vi) continue;
                for (idx_t m = 0; ; ++m) {
                    /* exponent e = d * m * m */
                    idx_t e = d * m * m;
                    if (e > cache_size - 1 - i) break;   /* beyond needed range */
                    seq_t coef = (m == 0) ? (seq_t)1 : (seq_t)2;
                    tmp[i + e] += vi * coef;
                }
            }

            /* Prepare for next divisor.  The previous polynomial
               'cur' is no longer needed. */
            free(cur);
            cur = tmp;
        }

        /* Finalised polynomial stored in 'cur'.  Put it into the
           cache for future queries. */
        cache = cur;
    }

    /* Return the requested coefficient. */
    return cache[n];
}
