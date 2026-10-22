/*
 * A001798  Coefficient of P_3(x) in the Legendre expansion of x^n.
 * https://oeis.org/A001798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = (14*n/((2*n+3)*(2*n+5)))*numerator(binomial(4*n+2,
 *            2*n+1)/2^(4*n)), evaluated as a product of prime powers p^e,
 *            with e found from Legendre/Kummer exponent counts
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   a001798_vp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a001798_vp.h"

seq_t a(idx_t n) {
    idx_t N = 4 * n + 5;
    idx_t m = 4 * n + 2, k = 2 * n + 1;
    unsigned char *comp = (unsigned char *)calloc((size_t)N + 1, 1);
    seq_t r = 1;
    idx_t p, q;
    for (p = 2; p <= N; p++) {
        long long e;
        if (comp[p]) continue;
        for (q = p * p; q <= N; q += p) comp[q] = 1;
        if (p == 2) {
            /* binomial has 2-adic valuation s(n)+1, which is fully removed
               by taking the numerator; only 14*n contributes */
            e = a001798_vp(n, 2) + 1;
        } else {
            idx_t pk = p;
            e = a001798_vp(14 * n, p);
            while (pk <= m) {
                e += (long long)(m / pk) - 2 * (long long)(k / pk);
                if (pk > m / p) break;
                pk *= p;
            }
            e -= a001798_vp(2 * n + 3, p) + a001798_vp(2 * n + 5, p);
        }
        while (e-- > 0) r *= (seq_t)p;
    }
    free(comp);
    return r;
}
