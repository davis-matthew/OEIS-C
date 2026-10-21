/*
 * A236283  The number of orbits of triples of {1,2,...,n} under the action of
 *          the dihedral group of order 2n.
 * https://oeis.org/A236283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5 + 3*(-1)^n + 2*n^2)/4
 * Range:     u8 n <= 15, u16 n <= 267, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    if ((n & 1ULL) == 1ULL)      /* n is odd */
        return (n2 + 1) / 2;
    else                         /* n is even */
        return n2 / 2 + (seq_t)2;
}
