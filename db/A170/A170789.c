/*
 * A170789  a(n) = n^9*(n^7 + 1)/2.
 * https://oeis.org/A170789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^9*(n^7 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 16, u128 n <= 267,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* convert index to seq_t once */
    seq_t nn = (seq_t)n;

    /* compute powers of n */
    seq_t n2 = nn * nn;          /* n^2   */
    seq_t n4 = n2 * n2;          /* n^4   */
    seq_t n8 = n4 * n4;          /* n^8   */
    seq_t n9 = n8 * nn;          /* n^9   */
    seq_t n6 = n4 * n2;          /* n^6   */
    seq_t n7 = n6 * nn;          /* n^7   */

    /* split the even factor to avoid overflow in intermediate product */
    if ((nn & 1) == 0) {         /* n is even */
        seq_t half = nn / 2;
        /* term = (n/2) * n^8 * (n^7 + 1) */
        return half * n8 * (n7 + 1);
    } else {                    /* n is odd */
        seq_t half = (n7 + 1) / 2;
        /* term = n^9 * ((n^7 + 1)/2) */
        return n9 * half;
    }
}
