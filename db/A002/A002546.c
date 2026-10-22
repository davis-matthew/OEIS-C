/*
 * A002546  Denominator of Sum_{i+j+k=n; i,j,k > 0} 1/(i*j*k).
 * https://oeis.org/A002546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of 6*|Stirling1(n+2,3)|/(n+2)! (coefficient
 *            of x^(n+2) in (-log(1-x))^3); computed prime by prime: exponent
 *            of p is max(0, v_p((n+2)!) - v_p(6*|Stirling1(n+2,3)|))
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   a2546_is_big, a2546_legendre, a2546_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a2546_is_big.h"
#include "../utils/a2546_legendre.h"
#include "../utils/a2546_sieve.h"

seq_t a(idx_t n) {
    idx_t m = n + 2, p, i, k;
    unsigned char *comp = a2546_sieve(m);
    seq_t res = 1;

    if (a2546_is_big()) {
        seq_t c1 = 1, c2 = 0, c3 = 0, N;
        for (i = 1; i < m; i++) {
            c3 = c2 + (seq_t)i * c3;
            c2 = c1 + (seq_t)i * c2;
            c1 = (seq_t)i * c1;
        }
        N = 6 * c3;
        for (p = 2; p <= m; p++) {
            idx_t e, v = 0;
            if (comp[p]) continue;
            e = a2546_legendre(m, p);
            if (N % (seq_t)p == 0) {
                seq_t q = N;
                while (v < e && q % (seq_t)p == 0) { q /= (seq_t)p; v++; }
            }
            for (k = v; k < e; k++) res *= (seq_t)p;
        }
    } else {
        for (p = 2; p <= m; p++) {
            idx_t e, v6, K, Kc, v;
            uint64_t pk, r;
            unsigned __int128 c1, c2, c3;
            if (comp[p]) continue;
            e = a2546_legendre(m, p);
            v6 = (p == 2 || p == 3) ? 1 : 0;
            if (e <= v6) continue;
            K = e - v6;
            pk = 1; Kc = 0;
            while (Kc < K && pk <= ((uint64_t)1 << 62) / p) { pk *= p; Kc++; }
            c1 = 1 % pk; c2 = 0; c3 = 0;
            for (i = 1; i < m; i++) {
                unsigned __int128 im = (unsigned __int128)(i % pk);
                c3 = (c2 + im * c3) % pk;
                c2 = (c1 + im * c2) % pk;
                c1 = (im * c1) % pk;
            }
            r = (uint64_t)c3;
            v = 0;
            if (r == 0) v = Kc;
            else while (v < Kc && r % p == 0) { r /= p; v++; }
            for (k = v; k < K; k++) res *= (seq_t)p;
        }
    }
    free(comp);
    return res;
}
