/*
 * A007757  Dwork-Kontsevich sequence evaluated at 2*n.
 * https://oeis.org/A007757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n)!/denominator(H(2n)) = (2n)!/A002805(2n), where H(m)
 *            = Sum_{k=1..m} 1/k (Krattenthaler-Rivoal formula). For each
 *            prime p <= 2n, v_p(denominator(H(2n))) is found from H(2n)
 *            computed modulo p^e, where p^e <= 2n < p^(e+1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 20,
 *            big any n
 * Helpers:   a007757_inv
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a007757_inv.h"

seq_t a(idx_t n)
{
    uint64_t N = 2 * (uint64_t)n;
    uint64_t p, i, m;
    seq_t res = 1;
    uint64_t chunk = 1;
    char *comp = (char *)calloc((size_t)(N + 1), 1);

    for (p = 2; p <= N; p++) {
        uint64_t M, e, vf, t, T, K, dv, ex;
        uint64_t pows[64];
        if (comp[p]) continue;
        for (i = p * p; i <= N; i += p) comp[i] = 1;

        M = 1; e = 0;
        pows[0] = 1;
        while (M * p <= N) { M *= p; e++; pows[e] = M; }

        vf = 0; t = N;
        while (t) { t /= p; vf += t; }

        /* T = Sum_{m=1..N/p} p^(e-1-v_p(m)) * (m / p^v_p(m))^(-1)  mod p^e */
        T = 0; K = N / p;
        for (m = 1; m <= K; m++) {
            uint64_t mm = m, v = 0, term;
            while (mm % p == 0) { mm /= p; v++; }
            term = (pows[e - 1 - v] * (uint64_t)a007757_inv((int64_t)(mm % M), (int64_t)M)) % M;
            T = (T + term) % M;
        }

        if (T == 0) dv = 0;
        else {
            uint64_t v = 0;
            while (T % p == 0) { T /= p; v++; }
            dv = e - v;
        }

        ex = vf - dv;
        for (i = 0; i < ex; i++) {
            if (chunk > UINT64_MAX / p) { res *= (idx_t)chunk; chunk = 1; }
            chunk *= p;
        }
    }
    res *= (idx_t)chunk;
    free(comp);
    return res;
}
