/*
 * A186636  a(n) = n*(n^3 + n^2 + 2*n + 1).
 * https://oeis.org/A186636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^3 + n^2 + 2*n + 1)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;               /* cast index to sequence type */
    seq_t n2 = tn * tn;                /* n^2 */
    seq_t n3 = n2 * tn;                /* n^3 */
    seq_t val = tn * (n3 + n2 + (seq_t)2 * tn + (seq_t)1);
    return val;
}
