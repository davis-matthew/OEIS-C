/*
 * A110688  Expansion of
 *          -(2*x+1)*(6*x^2+4*x+1)/((3*x^2+3*x+1)*(2*x^3+2*x^2+4*x+1)).
 * https://oeis.org/A110688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the expansion of
 *            -(2*x+1)*(6*x^2+4*x+1)/((3*x^2+3*x+1)*(2*x^3+2*x^2+4*x+1)). It
 *            satisfies the linear recurrence: a(n) = -7*a(n-1) -17*a(n-2)
 *            -20*a(n-3) -12*a(n-4) - 6*a(n-5) for n >= 5, with initial values
 *            a(0) = -1, a(1) = 1, a(2) = -4, a(3) = 19, a(4) = -73
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 17, i64 n <= 34, i128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return -1;
    if (n == 1) return 1;
    if (n == 2) return -4;
    if (n == 3) return 19;
    if (n == 4) return -73;

    /* Recurrent computation for n >= 5. */
    seq_t a0 = -1, a1 = 1, a2 = -4, a3 = 19, a4 = -73;
    seq_t an = 0;
    for (idx_t i = 5; i <= n; ++i) {
        an = -((seq_t)7 * a4
               + (seq_t)17 * a3
               + (seq_t)20 * a2
               + (seq_t)12 * a1
               + (seq_t)6  * a0);
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return an;
}
