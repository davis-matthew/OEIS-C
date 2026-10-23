/*
 * A143021  Number of vertices of degree 1 in all non-crossing connected
 *          graphs on n points on a circle.
 * https://oeis.org/A143021
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*A089436(n) Recurrence (valid for n≥6): (n-1)*(n-2)*a(n)
 *            -34*(n-2)*(n-4)*a(n-1) +4*(29*n^2-396*n+937)*a(n-2)
 *            +24*(153*n^2-1071*n+1810)*a(n-3)
 *            -2688*(3*n-14)*(3*n-16)*a(n-4)=0 Initial values: a(2)=2, a(3)=6,
 *            a(4)=36, a(5)=270
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 40,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t max_computed = 1;      /* highest index already in cache */

    /* make room for any index up to n */
    ensure_cache(n, &cache, &cache_cap);

    /* initialise base cases once */
    if (cache[2] == (seq_t)0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)36;
        cache[5] = (seq_t)270;
        max_computed = 5;
    }

    if (n <= max_computed)
        return cache[n];

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* recurrence needed only for i ≥ 6 */
        if (i < 6) {
            cache[i] = (seq_t)0;  /* should not happen */
            continue;
        }

        /* Branch on whether seq_t is small (≤ 64‑bit) or big */
        if (sizeof(seq_t) <= sizeof(unsigned long long)) {
            /* Use signed 128‑bit for all intermediate calculations */
            __int128 num = 0;

            num += (__int128)cache[i-1] * 34 * (i-2) * (i-4);
            num += (__int128)cache[i-4] * 2688 * (3*i-14) * (3*i-16);

            __int128 coeff2 = 4 * ((__int128)29 * i * i - (__int128)396 * i + 937);
            num -= (__int128)cache[i-2] * coeff2;

            __int128 coeff3 = 24 * ((__int128)153 * i * i - (__int128)1071 * i + 1810);
            num -= (__int128)cache[i-3] * coeff3;

            __int128 denom = (__int128)(i-1) * (i-2);
            cache[i] = (seq_t)(num / denom);
        } else {
            /* seq_t is an arbitrary‑precision class – use its operations */
            seq_t num = cache[i-1] * 34;
            num *= (i-2);
            num *= (i-4);

            num += cache[i-4] * 2688 * (3*i-14) * (3*i-16);

            long long coeff2 = 4 * ((29LL*i*i - 396LL*i + 937LL));
            if (coeff2 >= 0)
                num -= cache[i-2] * (seq_t)coeff2;
            else
                num += cache[i-2] * (seq_t)(-coeff2);

            long long coeff3 = 24 * ((153LL*i*i - 1071LL*i + 1810LL));
            if (coeff3 >= 0)
                num -= cache[i-3] * (seq_t)coeff3;
            else
                num += cache[i-3] * (seq_t)(-coeff3);

            cache[i] = num / ((seq_t)(i-1) * (i-2));
        }
    }

    max_computed = n;
    return cache[n];
}
