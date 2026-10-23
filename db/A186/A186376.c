/*
 * A186376  a(n) is the sum of the squares of the coefficients of (x + 2*y +
 *          3*z)^n.
 * https://oeis.org/A186376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the squares of the coefficients of (x + 2*y +
 *            3*z)^n Recurrence: a(0) = 1, a(1)=14, a(2)=294 For n >= 3:
 *            n^2*(4*n-7)*a(n) = 14*(16*n^3-44*n^2+34*n-9)*a(n-1) -
 *            196*(2*n-3)^2*(4*n-3)*a(n-2) +
 *            144*(4*n-9)*(4*n-7)*(4*n-3)*a(n-3)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cached = 0;            /* highest index computed */
    static idx_t cap = 0;               /* capacity of cache */

    /* progressive cache allocation */
    if (cache == NULL) {
        cap = 3;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)14;
        cache[2] = (seq_t)294;
        cached = 2;
    }

    /* extend cache if necessary */
    if (n > cached) {
        if (n + 1 >= cap) {
            size_t newcap = (size_t)((n + 1) > (cap * 2) ? (n + 1) : (cap * 2));
            seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) exit(1);          /* abort on out‑of‑memory */
            cache = tmp;
            cap = newcap;
        }
        /* fill new values using the recurrence */
        for (idx_t i = cached + 1; i <= n; ++i) {
            idx_t nn = i;
            /* Determine if seq_t is small (<=64 bits) to use an
               unsigned __int128 temporary to avoid overflow.  */
            if (sizeof(seq_t) <= 8) {
                __int128 t1 = (__int128)16 * nn * nn * nn
                            - (__int128)44 * nn * nn
                            + (__int128)34 * nn
                            - 9;
                __int128 coeff2 = (__int128)(2 * nn - 3);
                coeff2 *= coeff2;                 /* (2n-3)^2 */
                coeff2 *= (4 * nn - 3);
                coeff2 *= 196;

                __int128 coeff3 = (__int128)(4 * nn - 9);
                coeff3 *= (4 * nn - 7);
                coeff3 *= (4 * nn - 3);
                coeff3 *= 144;

                __int128 num = (__int128)14 * t1 * ( (__int128)cache[i-1] );
                num -= coeff2 * ( (__int128)cache[i-2] );
                num += coeff3 * ( (__int128)cache[i-3] );

                __int128 denom = (__int128)nn * nn * (4 * nn - 7);
                cache[i] = (seq_t)(num / denom);
            } else {
                /* seq_t is large (e.g., 128‑bit or arbitrary‑precision) */
                seq_t t1 = (seq_t)16 * nn * nn * nn
                         - (seq_t)44 * nn * nn
                         + (seq_t)34 * nn
                         - 9;
                seq_t coeff2 = (seq_t)(2 * nn - 3);
                coeff2 = coeff2 * coeff2 * (4 * nn - 3) * 196;
                seq_t coeff3 = (seq_t)(4 * nn - 9);
                coeff3 = coeff3 * (4 * nn - 7) * (4 * nn - 3) * 144;

                seq_t num = 14 * t1 * cache[i-1] - coeff2 * cache[i-2]
                              + coeff3 * cache[i-3];
                seq_t denom = nn * nn * (4 * nn - 7);
                cache[i] = num / denom;
            }
        }
        cached = n;
    }
    return cache[n];
}
