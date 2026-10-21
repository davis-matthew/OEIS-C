/*
 * A201838  G.f.: imaginary part of 1/(1 - i*x - i*x^2) where i=sqrt(-1).
 * https://oeis.org/A201838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-2) - 2*a(n-3) - a(n-4) (n >= 4) Initial values:
 *            a(0)=0, a(1)=1, a(2)=1, a(3)=-1
 * Range:     i8 n <= 14, i16 n <= 31, i32 n <= 59, i64 n <= 120,
 *            i128 n <= 241, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* quick return for small n */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)-1;

    /* iterative recurrence using the last four values:
       prev0 = a(i-4), prev1 = a(i-3), prev2 = a(i-2), prev3 = a(i-1)
    */
    seq_t prev0 = (seq_t)0;    /* a0   */
    seq_t prev1 = (seq_t)1;    /* a1   */
    seq_t prev2 = (seq_t)1;    /* a2   */
    seq_t prev3 = (seq_t)-1;   /* a3   */
    seq_t an;                  /* a(i) */

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = -a(i-2) - 2*a(i-3) - a(i-4) */
        an = -prev2 - (prev1 * (seq_t)2) - prev0;

        /* shift the window for next iteration */
        prev0 = prev1;
        prev1 = prev2;
        prev2 = prev3;
        prev3 = an;
    }

    return an;
}
