/*
 * A000116  Number of even sequences with period 2n (bisection of A000013).
 * https://oeis.org/A000116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = A000013(2n); for n >= 1, a(n) = (1/(4n)) * Sum_{d | 2n}
 *            phi(2d) * 2^(2n/d); a(0) = 1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   a116_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a116_phi.h"

seq_t a(idx_t n)
{
    uint64_t m, M, d;
    seq_t Q = 0;
    uint64_t R = 0;

    if (n == 0) return (seq_t)1;
    m = 2 * (uint64_t)n;
    M = 2 * m;
    for (d = 1; d <= m; d++) {
        uint64_t e, i, r, ph, pr;
        seq_t q;
        if (m % d != 0) continue;
        e = m / d;
        /* q = floor(2^e / M), r = 2^e mod M, by binary long division */
        q = 0;
        r = 1;
        for (i = 0; i < e; i++) {
            r *= 2;
            q *= 2;
            if (r >= M) { r -= M; q += 1; }
        }
        ph = a116_phi(2 * d);
        pr = ph * r;
        Q += q * (seq_t)ph + (seq_t)(pr / M);
        R += pr % M;
    }
    Q += (seq_t)(R / M);
    return Q;
}
