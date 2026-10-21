/*
 * A077984  Expansion of 1/(1+2*x-2*x^2+2*x^3).
 * https://oeis.org/A077984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*a(n-1) + 2*a(n-2) - 2*a(n-3); a(0)=1, a(1)=-2, a(2)=6
 * Range:     i8 n <= 4, i16 n <= 10, i32 n <= 20, i64 n <= 41, i128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-2;
    if (n == 2) return (seq_t)6;

    /* Use a sliding window of the last three terms. */
    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)-2;  /* a(n-2) */
    seq_t a2 = (seq_t)6;   /* a(n-1) */
    seq_t a3;

    for (idx_t i = 3; i <= n; ++i) {
        a3 = -(seq_t)2 * a2 + (seq_t)2 * a1 - (seq_t)2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }

    return a2;
}
