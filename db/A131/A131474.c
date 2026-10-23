/*
 * A131474  a(n) = ceiling(n/2)*ceiling(n^2/2).
 * https://oeis.org/A131474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n/2) * ceil(n^2/2)
 * Range:     u8 n <= 10, u16 n <= 63, u32 n <= 2580, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn      = (seq_t)n;                /* cast index to sequence type */
    seq_t part1   = (nn + 1) / 2;            /* ceil(n/2)  */
    seq_t part2   = (nn * nn + 1) / 2;       /* ceil(n^2/2) */
    return part1 * part2;                   /* product */
}
