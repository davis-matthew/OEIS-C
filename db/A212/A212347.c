/*
 * A212347  Sequence of coefficients of x^1 in marked mesh pattern generating
 *          function Q_{n,132}^(0,4,0,0)(x).
 * https://oeis.org/A212347
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2-2*n-8)*(n^2-2*n-3)/6 (valid for n >= 5)
 * Range:     u8 n <= 7, u16 n <= 26, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For indices below the offset, the value is undefined; return 0. */
    if (n < 5) return (seq_t)0;

    seq_t nn = (seq_t)n;

    seq_t t1 = nn * nn - (seq_t)2 * nn - (seq_t)8;
    seq_t t2 = nn * nn - (seq_t)2 * nn - (seq_t)3;

    return (t1 * t2) / (seq_t)6;
}
