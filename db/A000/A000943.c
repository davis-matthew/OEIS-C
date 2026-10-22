/*
 * A000943  Number of combinatorial types of simplicial n-dimensional
 *          polytopes with n+3 nodes.
 * https://oeis.org/A000943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   with N = n+4 and T = floor((N-1)/2), a(n) = ( (1/N)*Sum_{d|N, d
 *            odd} phi(d)*2^(N/d-1) - 1 - A007997-like neck3(N) + 2^T - 1 - T
 *            )/2 + round((N-3)^2/12), where neck3(N) = (binomial(N-1,2) +
 *            2*[3|N])/3. This counts Perles' standard Gale diagrams: regular
 *            m-gons (m odd >= 3) whose vertices carry pos...
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   a943_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a943_phi.h"

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n + 4;
    uint64_t M2 = 2 * N;
    seq_t Q = 0;
    uint64_t R = 0;
    uint64_t d, i;

    /* Sum_{d|N, d odd} phi(d) 2^(N/d-1) = M2*Q + R */
    for (d = 1; d <= N; d += 2) {
        if (N % d != 0) continue;
        uint64_t e = N / d - 1;
        uint64_t ph = a943_phi(d);
        seq_t q = 0;
        uint64_t r = 1;
        for (i = 0; i < e; i++) {
            q = q * 2;
            r *= 2;
            if (r >= M2) { r -= M2; q += 1; }
        }
        q = q * (seq_t)ph;
        r *= ph;
        q += (seq_t)(r / M2);
        r %= M2;
        Q += q;
        R += r;
        if (R >= M2) { Q += (seq_t)(R / M2); R %= M2; }
    }
    uint64_t k0 = R / N; /* 0 or 1 */

    uint64_t T = (N - 1) / 2;
    seq_t P = 1;
    for (i = 0; i + 1 < T; i++) P = P * 2;   /* 2^(T-1) */

    uint64_t neck3 = ((N - 1) * (N - 2) / 2 + (N % 3 == 0 ? 2 : 0)) / 3;
    uint64_t sub = (2 + T + neck3 - k0) / 2;
    uint64_t S = N - 3;
    uint64_t p3 = (S * S + 6) / 12;

    return Q + P + (seq_t)p3 - (seq_t)sub;
}
