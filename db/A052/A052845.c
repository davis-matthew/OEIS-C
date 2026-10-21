/*
 * A052845  Expansion of e.g.f.: exp(x^2/(1-x)).
 * https://oeis.org/A052845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=0, a(2)=2
 *            (n^2+3*n+2)*a(n)+(n^2+n-2)*a(n+1)+(-4-2*n)*a(n+2)+a(n+3)=0
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t   cap   = 0;   /* allocated capacity of cache   */
    static idx_t   last   = 2;   /* highest index already computed */

    /* Allocate/resize cache to store indices 0..n */
    if (cache == NULL) {
        cap = 3;                 /* store a[0], a[1], a[2] */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
    } else if (n + 1 > cap) {
        size_t newcap = cap ? cap : 1;
        while (newcap <= (size_t)n + 1) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cap = newcap;
    }

    if (n <= last) return cache[n];

    for (idx_t i = last + 1; i <= n; ++i) {
        idx_t k = i - 3;

        /* Coefficients – use signed int64_t for possible negative values */
        int64_t c0 = (int64_t)k * k + 3LL * k + 2LL;   /* always non‑negative for k >= 0 */
        int64_t c1 = (int64_t)k * k + 1LL * k - 2LL;   /* may be negative for k = 0,1 */
        int64_t c2 = 4LL + 2LL * k;                    /* non‑negative */

        seq_t term0 = cache[i - 3] * (seq_t)c0;
        seq_t term2 = cache[i - 1] * (seq_t)c2;
        seq_t result;

        if (c1 >= 0) {
            seq_t term1 = cache[i - 2] * (seq_t)c1;
            result = term2 - (term0 + term1);
        } else {
            seq_t term1p = cache[i - 2] * (seq_t)(-c1);
            result = term2 - term0 + term1p;
        }

        cache[i] = result;
    }

    last = n;
    return cache[n];
}
