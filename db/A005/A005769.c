/*
 * A005769  Number of convex polygons of length 2n on square lattice whose
 *          leftmost bottom vertex is strictly to the right of the rightmost
 *          top vertex.
 * https://oeis.org/A005769
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f. = x^4*P(x)/((1-4x)^2*(1-2x)*(1-3x+x^2)^2) -
 *            2*x^4*(1-4x)^(-3/2), with P(x) = 2 - 20x + 75x^2 - 127x^3 +
 *            95x^4 - 27x^5 + 4x^6 (the OEIS g.f. with
 *            (1-2x^(1/2))(1+2x^(1/2)) = 1-4x and (1+x^(1/2)-x)(1-x^(1/2)-x) =
 *            1-3x+x^2). So with m = n-4: a(n) = R(m) -
 *            2*(2m+1)*binomial(2m,m), where R(m) = P_...
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 18, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    static const int P[7] = {2, -20, 75, -127, 95, -27, 4};
    static const int C[8] = {0, 16, -103, 340, -605, 554, -224, 32};
    seq_t w[8];
    idx_t m = n - 4;
    idx_t k;
    int i;
    seq_t cb;

    for (k = 0; k <= m; k++) {
        seq_t pos = 0, neg = 0;
        if (k < 7) {
            if (P[k] >= 0) pos += (seq_t)P[k];
            else neg += (seq_t)(-P[k]);
        }
        for (i = 1; i <= 7; i++) {
            if (k >= (idx_t)i) {
                seq_t v = w[(k - (idx_t)i) % 8];
                if (C[i] >= 0) pos += v * (seq_t)C[i];
                else neg += v * (seq_t)(-C[i]);
            }
        }
        w[k % 8] = pos - neg;
    }
    cb = binom(2 * m, m);
    return w[m % 8] - (seq_t)2 * ((seq_t)2 * (seq_t)m + 1) * cb;
}
