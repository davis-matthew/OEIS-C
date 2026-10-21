/*
 * A066394  Coordination sequence for ReO_3 net with respect to oxygen atom
 *          O_1.
 * https://oeis.org/A066394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n>0: a(n) = n^2 + 1 if n is odd, * a(n) = 3*n^2 - 2
 *            if n is even
 * Range:     u8 n <= 9, u16 n <= 147, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t nn = (seq_t)n;
    if ((n & 1) != 0)   /* odd */
        return nn * nn + (seq_t)1;
    else                /* even */
        return (seq_t)3 * nn * nn - (seq_t)2;
}
