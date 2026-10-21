/*
 * A092841  Numerator of I(n) = Integral_{x=0..1/(4^n)} (1-sqrt(x))^2 dx;
 *          e.g., I(3) = 323/24576. The denominator is b(n) = 96*16^(n-1);
 *          e.g., b(3) = 24576.
 * https://oeis.org/A092841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 6·4ⁿ – 8·2ⁿ + 3 (Numerator of the integral I(n) with
 *            denominator 96·16ⁿ⁻¹)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow2 = 1;
    seq_t pow4 = 1;

    for (idx_t i = 0; i < n; ++i) {
        pow2 *= 2;   /* 2ⁿ  */
        pow4 *= 4;   /* 4ⁿ  */
    }

    seq_t res = 6 * pow4 - 8 * pow2 + 3;
    return res;
}
