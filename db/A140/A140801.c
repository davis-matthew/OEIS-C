/*
 * A140801  a(0)=360, a(n) = a(n-1) + 720 for n>=1.
 * https://oeis.org/A140801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 360 + 720*n
 * Range:     u8 none, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)360 + (seq_t)720 * (seq_t)n;
}
