/*
 * A131895  a(n) = (n + 2)*(5*n + 1)/2.
 * https://oeis.org/A131895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 2)*(5*n + 1)/2
 * Range:     u8 n <= 9, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* convert index to seq_t for safety */
    return (nn + 2) * (5*nn + 1) / 2;
}
