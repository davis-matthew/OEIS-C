/*
 * A077940  Expansion of 1/(1-2*x+2*x^3).
 * https://oeis.org/A077940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3), with initial values a(0)=1, a(1)=2,
 *            a(2)=4
 * Range:     i8 n <= 10, i16 n <= 23, i32 n <= 48, i64 n <= 101,
 *            i128 n <= 202, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;

    /* Iteratively compute values using the recurrence.
       Only need to keep the last three values: a(n-3), a(n-2), a(n-1). */
    seq_t a0 = 1;   /* a(n-3) */
    seq_t a1 = 2;   /* a(n-2) */
    seq_t a2 = 4;   /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i)
    {
        /* a(n) = 2*a(n-1) - 2*a(n-3) */
        seq_t an = (seq_t)2 * a2 - (seq_t)2 * a0;

        /* Slide the window forward: a0 <- a1, a1 <- a2, a2 <- an */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;   /* a(n) */
}
