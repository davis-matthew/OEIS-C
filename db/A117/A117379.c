/*
 * A117379  Expansion of (1-4x)/(1-x^2+x^3).
 * https://oeis.org/A117379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-2) - a(n-3) for n >= 3 with initial values a(0)=1,
 *            a(1)=-4, a(2)=1
 * Range:     i8 n <= 15, i16 n <= 35, i32 n <= 36, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases directly from the generating function */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)-4;
    if (n == 2u) return (seq_t)1;

    /* a0 = a(n-3), a1 = a(n-2), a2 = a(n-1) for the current n */
    seq_t a0 = (seq_t)1;      /* a(0)   */
    seq_t a1 = (seq_t)-4;     /* a(1)   */
    seq_t a2 = (seq_t)1;      /* a(2)   */

    for (idx_t i = 3u; i <= n; ++i)
    {
        seq_t ai = a1 - a0;   /* a(i) = a(i-2) - a(i-3) */
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
