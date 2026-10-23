/*
 * A033571  a(n) = (2*n + 1)*(5*n + 1).
 * https://oeis.org/A033571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n + 1)*(5*n + 1)
 * Range:     u8 n <= 4, u16 n <= 80, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return (sn * 2 + 1) * (sn * 5 + 1);
}
