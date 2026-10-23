/*
 * A159502  Numerator of Hermite(n, 9/13).
 * https://oeis.org/A159502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0) = 1 * a(1) = 18 * a(n) = 18*a(n-1) - 2*(n-1)*169*a(n-2) for
 *            n >= 2
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    if (n == 1)
        return (seq_t)18;

    seq_t a_im1 = (seq_t)1;   /* a(0) */
    seq_t a_i   = (seq_t)18;  /* a(1) */

    for (idx_t i = 1; i <= n - 1; ++i) {
        /* a_{i+1} = 18*a_i - 2*i*169*a_im1 */
        seq_t a_next = (seq_t)18 * a_i
                     - (seq_t)(2 * i) * (seq_t)169 * a_im1;
        a_im1 = a_i;
        a_i   = a_next;
    }

    return a_i;
}
