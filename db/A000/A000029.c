/*
 * A000029  Number of necklaces with n beads of 2 colors, allowing turning
 *          over (these are also called bracelets).
 * https://oeis.org/A000029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, core
 * Formula:   a(0) = 1; a(n) = Sum_{d|n} phi(d)*2^(n/d)/(2*n) + (2^((n-1)/2)
 *            if n odd, else 2^(n/2-1) + 2^(n/2-2))
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 38, u64 n <= 71, u128 n <= 136,
 *            big any n
 * Helpers:   phi_ull, pow2_s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi_ull.h"
#include "../utils/pow2_s.h"

seq_t a(idx_t n)
{
    unsigned long long M, R, d;
    seq_t Q, half;

    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 3;

    /* Q*M + R accumulates Sum_{d|n} phi(d)*2^(n/d), with M = 2n */
    M = 2ULL * (unsigned long long)n;
    Q = 0;
    R = 0;
    for (d = 1; d <= (unsigned long long)n; d++) {
        unsigned long long m, i, r, ph;
        seq_t q;
        if ((unsigned long long)n % d != 0) continue;
        m = (unsigned long long)n / d;
        q = 0;
        r = 1;
        for (i = 0; i < m; i++) {
            q += q;
            r += r;
            if (r >= M) {
                r -= M;
                q += 1;
            }
        }
        ph = phi_ull(d);
        Q += q * (seq_t)ph;
        R += r * ph;
        Q += (seq_t)(R / M);
        R %= M;
    }

    if (n & 1)
        half = pow2_s(((unsigned long long)n - 1) / 2);
    else
        half = pow2_s((unsigned long long)n / 2 - 2) * 3;

    return Q + half;
}
