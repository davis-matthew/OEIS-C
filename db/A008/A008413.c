/*
 * A008413  Coordination sequence for 5-dimensional cubic lattice.
 * https://oeis.org/A008413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = (4/3)*n^4 + (20/3)*n^2 + 2 for n > 0
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 237, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;

    seq_t t  = (seq_t)n;          /* n as seq_t */
    seq_t n2 = t * t;             /* n^2   */
    seq_t n4 = n2 * n2;           /* n^4   */

    return ( ( (seq_t)4 * n4 + (seq_t)20 * n2 ) / (seq_t)3 ) + (seq_t)2;
}
