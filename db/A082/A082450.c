/*
 * A082450  a(n) = 5*(n^2 - n + 2)/2.
 * https://oeis.org/A082450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*(n^2 - n + 2)/2
 * Range:     u8 n <= 10, u16 n <= 162, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* convert index to sequence type */
    seq_t val = nn * nn - nn + 2;/* n^2 - n + 2  (no overflow for required n) */
    return (seq_t)5 * val / 2;    /* multiply by 5 and divide by 2 */
}
