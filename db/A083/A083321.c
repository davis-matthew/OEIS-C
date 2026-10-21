/*
 * A083321  a(n) = (-1)^n + (-2)^n - (-3)^n.
 * https://oeis.org/A083321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (-1)^n + (-2)^n - (-3)^n Equivalent recurrence: a(0)=1,
 *            a(1)=0, a(2)=-4, a(n) = -6*a(n-1) - 11*a(n-2) - 6*a(n-3) for
 *            n>=3
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 19, i64 n <= 39, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)0;   /* a(1) */
    seq_t a2 = (seq_t)-4;  /* a(2) */

    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;

    /* Iteratively apply the recurrence up to n */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t a3 = (seq_t)-6 * a2 + (seq_t)-11 * a1 + (seq_t)-6 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a2;
}
