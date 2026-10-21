/*
 * A054604  a(n) = Sum_{d|5} phi(d)*n^(5/d).
 * https://oeis.org/A054604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 + 4*n
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 84, u64 n <= 7131,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m   = (seq_t)n;      /* cast index to the sequence type */
    seq_t val = m * m;         /* n^2 */
    val *= m;                  /* n^3 */
    val *= m;                  /* n^4 */
    val *= m;                  /* n^5 */
    val += 4 * m;              /* add 4n */
    return val;
}
