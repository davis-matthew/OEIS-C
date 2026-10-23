/*
 * A195038  41 times triangular numbers.
 * https://oeis.org/A195038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 41*n*(n+1)/2
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = nn * (nn + 1);
    val /= 2;          /* compute the triangular number */
    val *= 41;         /* multiply by 41 */
    return val;
}
