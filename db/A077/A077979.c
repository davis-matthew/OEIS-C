/*
 * A077979  Expansion of 1/(1+x+2*x^2+x^3).
 * https://oeis.org/A077979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) - 2*a(n-2) - a(n-3) with initial values a(0)=1,
 *            a(1)=-1, a(2)=-1
 * Range:     i8 n <= 17, i16 n <= 37, i32 n <= 77, i64 n <= 156,
 *            i128 n <= 314, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-1;
    if (n == 2) return (seq_t)-1;

    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)-1;  /* a(n-2) */
    seq_t a2 = (seq_t)-1;  /* a(n-1) */
    seq_t a_curr;

    for (idx_t k = 3; k <= n; ++k) {
        a_curr = -(a2 + (seq_t)2 * a1 + a0);
        a0 = a1;
        a1 = a2;
        a2 = a_curr;
    }
    return a2;
}
