/*
 * A002702  Coefficients for numerical differentiation.
 * https://oeis.org/A002702
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of 12*H(n-1,2)/(n*binomial(2n,n)), where
 *            H(m,2) = Sum_{k=1..m} 1/k^2. This is the coefficient of
 *            mu*delta^(2n-1) in (hD)^3 = 8*asinh(delta/2)^3, i.e. the same as
 *            den(6*A002455(n)/(2^(2n)*(2n+3)!)). The denominator is built
 *            prime by prime from p-adic valuations
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   a002702_mm, a002702_vfact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a002702_mm.h"
#include "../utils/a002702_vfact.h"

seq_t a(idx_t n) {
    if (n < 2) return 0;
    uint64_t lim = 2 * (uint64_t)n;
    unsigned char *comp = (unsigned char *)calloc(lim + 1, 1);
    seq_t res = 1;
    for (uint64_t p = 2; p <= lim; p++) {
        if (comp[p]) continue;
        for (uint64_t q = p * p; q <= lim; q += p) comp[q] = 1;

        long long vpn = 0;
        { uint64_t t = n; while (t % p == 0) { t /= p; vpn++; } }
        long long vC = a002702_vfact(lim, p) - 2 * a002702_vfact(n, p);
        long long v12 = (p == 2) ? 2 : (p == 3) ? 1 : 0;
        long long K = vpn + vC - v12;

        long long m = 0;
        { uint64_t t = p; while (t <= (uint64_t)n - 1) { m++; t *= p; } }
        long long M = 2 * m + K;
        if (M <= 0) continue;

        uint64_t P = 1;
        for (long long i = 0; i < M; i++) P *= p;
        uint64_t pp[140];
        {
            uint64_t c = 1;
            for (long long e = 0; e <= 2 * m; e++) {
                pp[e] = (e >= M) ? 0 : c;
                if (e < M) c *= p;
            }
        }

        uint64_t A = 0, B = 1;
        for (uint64_t k = 1; k <= (uint64_t)n - 1; k++) {
            uint64_t kk = k;
            long long v = 0;
            if (kk % p == 0) { while (kk % p == 0) { kk /= p; v++; } }
            uint64_t s = kk % P;
            s = a002702_mm(s, s, P);
            A = (a002702_mm(A, s, P) + a002702_mm(pp[2 * (m - v)], B, P)) % P;
            B = a002702_mm(B, s, P);
        }

        long long vT;
        if (A == 0) vT = M;
        else { vT = 0; while (A % p == 0) { A /= p; vT++; } }

        long long e = M - vT;
        for (long long i = 0; i < e; i++) res *= (seq_t)p;
    }
    free(comp);
    return res;
}
