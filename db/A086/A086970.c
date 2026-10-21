/*
 * A086970  Fix 1, then exchange the subsequent odd numbers in pairs.
 * https://oeis.org/A086970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n + 1 - 2*(-1)^n for n > 0, a(0) = 1 Equivalent
 *            piecewise: a(n) = (n%2==0) ? 2*n-1 : 2*n+3, a(0) = 1
 * Range:     u8 n <= 126, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 1;
    }
    seq_t nn = (seq_t)n;
    if (n & 1) {          /* n is odd */
        return (seq_t)(2*nn + 3);
    } else {              /* n is even */
        return (seq_t)(2*nn - 1);
    }
}
