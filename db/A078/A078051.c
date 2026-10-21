/*
 * A078051  Expansion of (1-x)/(1+x+2*x^2+x^3).
 * https://oeis.org/A078051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) - 2*a(n-2) - a(n-3), a(0)=1, a(1)=-2, a(2)=0
 * Range:     i8 n <= 18, i16 n <= 36, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values of the recurrence */
    seq_t a0 = 1;    /* a(0) */
    seq_t a1 = -2;   /* a(1) */
    seq_t a2 = 0;    /* a(2) */

    /* Return immediately for the first three indices */
    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;

    seq_t ai;   /* temporary variable for subsequent terms */

    /* Compute terms iteratively up to n */
    for (idx_t i = 3; i <= n; ++i) {
        ai = -a2 - ((seq_t)2) * a1 - a0;  /* a(i) = -(a(i-1)+2a(i-2)+a(i-3)) */
        /* Shift the last three terms */
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }

    return ai;
}
