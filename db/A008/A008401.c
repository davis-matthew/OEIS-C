/*
 * A008401  Coordination sequence for {E_6}* lattice.
 * https://oeis.org/A008401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0) = 1 and for n>0, a(n) = 6*n*(3*n^4 + 5*n^2 + 1)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 47, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;          /* n^2 */
    seq_t n4 = n2 * n2;          /* n^4 */
    seq_t expr = 3 * n4 + 5 * n2 + (seq_t)1;
    return 6 * sn * expr;
}
