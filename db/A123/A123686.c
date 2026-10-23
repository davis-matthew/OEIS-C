/*
 * A123686  E.g.f.: (1-x^4)^(-1/2)*exp(x^2/(1-x^2))*BesselI(0,x^2/(x^2-1))
 *          (since this is an even function, we do not give the intercalating
 *          0's).
 * https://oeis.org/A123686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence for A123686 n*a(n) = 2*(2n-1)*(3n-2)*a(n-1) +
 *            4*(n-1)^2*(2n-3)*(2n-1)*(2n+1)*a(n-2) +
 *            8*(n-2)^2*(n-1)*(2n-5)*(2n-3)*(2n-1)*a(n-3) -
 *            16*(n-3)^2*(n-2)^2*(n-1)*(2n-7)*(2n-5)*(2n-3)*(2n-1)*a(n-4) with
 *            a(0)=1, a(1)=2, a(2)=54, a(3)=2460
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

seq_t a(idx_t n) {
    if (n < cache_len)
        return cache[n];

    /* Initialize cache with first four terms */
    if (cache_len == 0) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)54;
        cache[3] = (seq_t)2460;
        cache_len = 4;
    }

    /* Grow cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap < n + 1)
            new_cap = new_cap ? new_cap * 2 : 4;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t ak;
        if (k < 4) {
            /* Use pre‑filled values */
            ak = cache[k];
        } else {
            seq_t a1 = cache[k-1];
            seq_t a2 = cache[k-2];
            seq_t a3 = cache[k-3];
            seq_t a4 = cache[k-4];

            seq_t term1 = (seq_t)2 * (seq_t)(2*k-1) * (seq_t)(3*k-2) * a1;
            seq_t term2 = (seq_t)4 * (seq_t)(k-1) * (seq_t)(k-1)
                          * (seq_t)(2*k-3) * (seq_t)(2*k-1)
                          * (seq_t)(2*k+1) * a2;
            seq_t term3 = (seq_t)8 * (seq_t)(k-2) * (seq_t)(k-2)
                          * (seq_t)(k-1) * (seq_t)(2*k-5)
                          * (seq_t)(2*k-3) * (seq_t)(2*k-1) * a3;
            seq_t term4 = (seq_t)16 * (seq_t)(k-3) * (seq_t)(k-3)
                          * (seq_t)(k-2) * (seq_t)(k-2)
                          * (seq_t)(k-1) * (seq_t)(2*k-7)
                          * (seq_t)(2*k-5) * (seq_t)(2*k-3)
                          * (seq_t)(2*k-1) * a4;

            seq_t num = term1 + term2 + term3 - term4;
            ak = num / (seq_t)k;
        }
        cache[k] = ak;
    }
    cache_len = n + 1;
    return cache[n];
}
