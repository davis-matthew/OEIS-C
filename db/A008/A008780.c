/*
 * A008780  a(n) = (n-dimensional partitions of 6) + C(n,4) + C(n,3).
 * https://oeis.org/A008780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (120 + 404*n + 490*n^2 + 255*n^3 + 50*n^4 + n^5)/120
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 101, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t = (seq_t)120 + (seq_t)404 * nn;
    t += (seq_t)490 * nn * nn;
    t += (seq_t)255 * nn * nn * nn;
    t += (seq_t)50 * nn * nn * nn * nn;
    t += nn * nn * nn * nn * nn;
    return t / (seq_t)120;
}
