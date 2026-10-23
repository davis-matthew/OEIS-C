/*
 * A115133  Partial sums of A064061.
 * https://oeis.org/A115133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=0}^6 coeff[i] * binomial(n+8-i, 8)
 * Range:     u8 none, u16 n <= 5, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    static const int coeffs[7] = {429, -2002, 4004, -4368, 2730, -924, 132};
    seq_t pos = 0;
    seq_t neg = 0;

    for (idx_t i = 0; i < 7; ++i) {
        idx_t top = n + 8 - i;
        if (top < 8) {
            continue;
        }
        seq_t bin = binomial(top, 8);
        if (bin == 0) {
            continue;
        }
        seq_t term = bin;
        int c = coeffs[i];
        if (c < 0) {
            term *= (seq_t)(-c);
            neg += term;
        } else if (c > 0) {
            term *= (seq_t)c;
            pos += term;
        }
    }
    return pos - neg;
}
