/*
 * A118015  a(n) = floor(n^2/5).
 * https://oeis.org/A118015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^2/5)
 * Range:     u8 n <= 35, u16 n <= 572, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* promote index to sequence type */
    return (nn * nn) / (seq_t)5; /* integer division truncates toward zero */
}
