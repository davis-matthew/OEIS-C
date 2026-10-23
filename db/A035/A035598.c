/*
 * A035598  Number of points of L1 norm 4 in cubic lattice Z^n.
 * https://oeis.org/A035598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2*(n^2 + 2)/3
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 265, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t tmp = n2 * (n2 + (seq_t)2);
    tmp = tmp * (seq_t)2;
    tmp /= (seq_t)3;
    return tmp;
}
