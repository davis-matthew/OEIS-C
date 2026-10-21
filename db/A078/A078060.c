/*
 * A078060  Expansion of (1-x)/(1+2*x-2*x^3).
 * https://oeis.org/A078060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -(2)*a(n-1) + (2)*a(n-3) for n >= 3, a(0) = 1, a(1) = -3,
 *            a(2) = 6. Origin: Expansion of (1-x)/(1+2*x-2*x^3)
 * Range:     i8 n <= 9, i16 n <= 23, i32 n <= 39, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-3;
    if (n == 2) return (seq_t)6;

    /* Keep only the last three values to compute the recurrence. */
    seq_t a0 = (seq_t)1;    /* a(n-3)   */
    seq_t a1 = (seq_t)-3;   /* a(n-2)   */
    seq_t a2 = (seq_t)6;    /* a(n-1)   */
    seq_t an = a2;          /* placeholder for current value */

    for (idx_t i = 3; i <= n; ++i) {
        an = -( (seq_t)2 * a2 ) + ( (seq_t)2 * a0 );  /* a(i) = -2*a(i-1) + 2*a(i-3) */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }

    return a2;
}
