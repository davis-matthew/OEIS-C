/*
 * A033415  a(n) = floor(95/n).
 * https://oeis.org/A033415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(95/n)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* return floor(95/n).  for n==0 return 0 to avoid division by zero */
    return n == 0 ? (seq_t)0 : (seq_t)95 / (seq_t)n;
}
