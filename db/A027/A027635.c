/*
 * A027635  Expansion of (1-x^8)*(1+x^5)/(1-x^2)^5.
 * https://oeis.org/A027635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{s∈{0,5,8,13}} sign(s) * C(4 + (n‑s)/2, 4) where
 *            sign(0)=+1, sign(5)=+1, sign(8)=‑1, sign(13)=‑1. A term is
 *            counted only if n ≥ s and (n‑s) is even
 * Range:     u8 n <= 7, u16 n <= 45, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    static const idx_t shifts[4] = {0, 5, 8, 13};
    static const int   signs[4]  = { 1, 1, -1, -1};

    seq_t pos = 0;          /* sum of positive contributions   */
    seq_t neg = 0;          /* sum of negative contributions  */

    for (int i = 0; i < 4; ++i) {
        if (n >= shifts[i] && ((n - shifts[i]) & 1ULL) == 0ULL) {
            idx_t k = (n - shifts[i]) / 2;
            seq_t term = binomial(4 + k, 4);   /* C(4+k,4) */
            if (signs[i] > 0)
                pos += term;
            else
                neg += term;
        }
    }
    return pos - neg;
}
