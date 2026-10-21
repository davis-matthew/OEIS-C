/*
 * A085280  Expansion of (1-4x+x^2)/((1-x)(1-3x)(1-4x)).
 * https://oeis.org/A085280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n + (4^n − 1)/3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow3 = 1;
    seq_t pow4 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow3 *= 3;
        pow4 *= 4;
    }
    seq_t term = (pow4 - 1) / 3;
    return pow3 + term;
}
