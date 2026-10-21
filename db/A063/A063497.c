/*
 * A063497  Number of atoms in first n shells of type I hyperfullerene.
 * https://oeis.org/A063497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*n^3 + 30*n^2 + 10*n
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 598, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t nsq = nn * nn;      /* n^2 */
    seq_t ncub = nsq * nn;    /* n^3 */
    return ncub * 20 + nsq * 30 + nn * 10;
}
