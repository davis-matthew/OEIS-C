/*
 * A061349  Sum of antidiagonals of A060736.
 * https://oeis.org/A061349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n*(13 + 3*(-1)^n + 14*n^2))/24 (= (7*n^3 + 5*n)/12 for
 *            odd n, (7*n^3 + 8*n)/12 for even n)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;

    if ((n & 1) == 0) {           /* n even */
        return ( ((seq_t)7 * n3) + ((seq_t)8 * nn) ) / (seq_t)12;
    } else {                       /* n odd */
        return ( ((seq_t)7 * n3) + ((seq_t)5 * nn) ) / (seq_t)12;
    }
}
