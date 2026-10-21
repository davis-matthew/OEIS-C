/*
 * A214733  a(n) = -a(n-1) - 3*a(n-2) with n>1, a(0)=0, a(1)=1.
 * https://oeis.org/A214733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) - 3*a(n-2) (a(0)=0, a(1)=1)
 * Range:     i8 n <= 10, i16 n <= 20, i32 n <= 40, i64 n <= 81, i128 n <= 162,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    seq_t a0 = 0;  /* a(0)   */
    seq_t a1 = 1;  /* a(1)   */
    seq_t an;     /* temporary storage for a(i) */

    for (idx_t i = 2; i <= n; ++i) {
        an = -a1 - 3 * a0; /* a(i) = -a(i-1) - 3*a(i-2) */
        a0 = a1;
        a1 = an;
    }
    return a1; /* a(n) */
}
