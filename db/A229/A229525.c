/*
 * A229525  Sum of coefficients of the transform a*x^2 + (4*a/k - b)*x +
 *          4*a/k^2 + 2*b/k + c = 0 for a,b,c = 1,-1,-1, k = 1,2,3...
 * https://oeis.org/A229525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + 6n + 4 if n is odd, * a(n) = (n^2 + 6n + 4)/4 if n
 *            is even * Equivalent to a(n) = -(-5+3*(-1)^n)*(4+6n+n^2)/8
 * Range:     u8 n <= 13, u16 n <= 253, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Convert index to seq_t for calculations */
    seq_t nn = (seq_t)n;

    /* s = n^2 + 6n + 4 */
    seq_t s = nn * nn + nn * 6 + 4;

    /* If n is odd, return s; else divide by 4 */
    if (n & 1) {
        return s;
    } else {
        return s / 4;
    }
}
