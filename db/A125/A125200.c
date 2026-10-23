/*
 * A125200  a(n) = n*(4*n^2 + n - 1)/2.
 * https://oeis.org/A125200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(4*n^2 + n - 1)/2
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* undefined for n < 1, return 0 for safety */

    /* cast index to seq_t for arithmetic */
    seq_t nn = (seq_t)n;

    /* compute tmp = 4*n^2 + n - 1 */
    seq_t tmp = (seq_t)4 * nn * nn + nn - (seq_t)1;

    /* split the division to avoid overflow:
       if n is even, divide n first (tmp is odd),
       if n is odd, divide tmp first (tmp is even). */
    if (nn & (seq_t)1)          /* n odd, tmp even */
        return nn * (tmp / (seq_t)2);
    else                        /* n even, tmp odd */
        return (nn / (seq_t)2) * tmp;
}
