/*
 * A212983  Number of (w,x,y) with all terms in {0,...,n} and w<=x+y and x<=y.
 * https://oeis.org/A212983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n^3 + 39*n^2 + 50*n + 24 - 3*(n mod 2))/24
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;                /* n^2 */
    seq_t n3 = n2 * ns;                /* n^3 */
    seq_t num = (seq_t)10 * n3 + (seq_t)39 * n2 + (seq_t)50 * ns + (seq_t)24;
    if (n & 1ull)                       /* if n is odd */
        num -= (seq_t)3;
    return num / (seq_t)24;             /* final division */
}
