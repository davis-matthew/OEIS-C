/*
 * A213390  Number of (w,x,y) with all terms in {0,...,n} and max(w,x,y) >=
 *          2*min(w,x,y).
 * https://oeis.org/A213390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n^3+24*n^2+21*n+8+3*n*(-1)^n)/8
 * Range:     u8 n <= 2, u16 n <= 20, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    /* Base polynomial: 6*n^3 + 24*n^2 + 21*n + 8 */
    seq_t base = (seq_t)6 * N * N * N
               + (seq_t)24 * N * N
               + (seq_t)21 * N
               + (seq_t)8;
    /* Add or subtract 3*n depending on parity of n */
    seq_t total;
    if ((n & (idx_t)1) == 0)          /* n even: add 3*n */
        total = base + (seq_t)3 * N;
    else                               /* n odd : subtract 3*n */
        total = base - (seq_t)3 * N;
    return total / (seq_t)8;
}
