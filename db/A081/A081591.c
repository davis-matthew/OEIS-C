/*
 * A081591  Third row of Pascal-(1,6,1) array A081581.
 * https://oeis.org/A081591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (49*n^2 - 21*n + 2)/2 (OEIS A081591)
 * Range:     u8 n <= 3, u16 n <= 51, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = (49 * nn * nn - 21 * nn + 2) / 2;
    return val;
}
