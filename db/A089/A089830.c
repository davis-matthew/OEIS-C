/*
 * A089830  Expansion of (1-3*x+6*x^2-5*x^3+3*x^4-x^5)/(1-x)^6.
 * https://oeis.org/A089830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (120 + 104*n + 110*n^2 + 15*n^3 + 10*n^4 + n^5) / 120
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n0 = (seq_t)n;                /* n^1  */
    seq_t n2 = n0 * n0;                 /* n^2  */
    seq_t n3 = n2 * n0;                 /* n^3  */
    seq_t n4 = n3 * n0;                 /* n^4  */
    seq_t n5 = n4 * n0;                 /* n^5  */

    seq_t num = 120
                + 104 * n0
                + 110 * n2
                + 15 * n3
                + 10 * n4
                + n5;

    return num / 120;
}
