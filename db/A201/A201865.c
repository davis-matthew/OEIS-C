/*
 * A201865  Expansion of 1/((1-3*x)*(1+7*x)).
 * https://oeis.org/A201865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (3^(n+1)+7*(-7)^n)/10
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 11, i64 n <= 22, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=-4, a(n) = -4*a(n-1)+21*a(n-2) */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    } else if (n == 1) {
        return (seq_t)-4;
    }

    seq_t a_n_minus_2 = (seq_t)1;   /* a(0) */
    seq_t a_n_minus_1 = (seq_t)-4;  /* a(1) */
    seq_t a_n = 0;

    for (idx_t i = 2; i <= n; ++i) {
        a_n = (seq_t)(-4) * a_n_minus_1 + (seq_t)21 * a_n_minus_2;
        a_n_minus_2 = a_n_minus_1;
        a_n_minus_1 = a_n;
    }

    return a_n;
}
