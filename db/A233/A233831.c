/*
 * A233831  a(n) = -2*a(n-1) -2*a(n-2) + a(n-3). a(0) = -1, a(1) = 1, a(2) =
 *          1.
 * https://oeis.org/A233831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -2*a(n-1) - 2*a(n-2) + a(n-3). a(0) = -1, a(1) = 1, a(2)
 *            = 1
 * Range:     i8 n <= 10, i16 n <= 19, i32 n <= 41, i64 n <= 85, i128 n <= 168,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)-1;
    if (n == 1 || n == 2)
        return (seq_t)1;

    seq_t a_im3 = (seq_t)-1; /* a(i-3) */
    seq_t a_im2 = (seq_t)1;  /* a(i-2) */
    seq_t a_im1 = (seq_t)1;  /* a(i-1) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t sum = a_im1 + a_im2;
        sum *= (seq_t)2;
        cur = a_im3 - sum;

        a_im3 = a_im2;
        a_im2 = a_im1;
        a_im1 = cur;
    }

    return a_im1;
}
