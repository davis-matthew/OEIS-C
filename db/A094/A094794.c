/*
 * A094794  a(n) = (1/n!)*A001689(n).
 * https://oeis.org/A094794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 + 10*n^4 + 45*n^3 + 100*n^2 + 109*n + 44
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t nn2 = nn * nn;
    seq_t nn3 = nn2 * nn;
    seq_t nn4 = nn3 * nn;
    seq_t nn5 = nn4 * nn;
    return nn5 + 10 * nn4 + 45 * nn3 + 100 * nn2 + 109 * nn + 44;
}
