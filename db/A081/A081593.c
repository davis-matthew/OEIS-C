/*
 * A081593  Third row of Pascal-(1,7,1) array A081582.
 * https://oeis.org/A081593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 32*n^2 - 16*n + 1
 * Range:     u8 n <= 3, u16 n <= 45, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;
    seq_t term = (seq_t)32 * t * t - (seq_t)16 * t + (seq_t)1;
    return term;
}
