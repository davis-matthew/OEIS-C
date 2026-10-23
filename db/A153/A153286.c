/*
 * A153286  a(n) = n^3 + sum((-1)^j*a(j)); for j=1 to n-1; a(1)=1.
 * https://oeis.org/A153286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^3 + Σ_{j=1}^{n-1} (-1)^j · a(j) Equivalent closed form:
 *            a(1) = 1 if n is even: a(n) = 3 n^2 – 3 n + 1 if n is odd >1:
 *            a(n) = 9 n^2 – 21 n + 15
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)1;
    }
    seq_t nn = (seq_t)n;
    if ((n & 1) == 0) {          /* n is even */
        return (seq_t)(3 * nn * nn - 3 * nn + 1);
    } else {                     /* n is odd, n > 1 */
        return (seq_t)(9 * nn * nn - 21 * nn + 15);
    }
}
