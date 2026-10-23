/*
 * A041010  Numerators of continued fraction convergents to sqrt(8).
 * https://oeis.org/A041010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 6*a(n-2) - a(n-4) for n >= 4; a(0) = 2, a(1) = 3, a(2) =
 *            14, a(3) = 17
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return the first four terms directly. */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)14;
    if (n == 3) return (seq_t)17;

    /* a0 = a(n-4), a1 = a(n-3), a2 = a(n-2), a3 = a(n-1) */
    seq_t a0 = (seq_t)2;   /* a(0)   */
    seq_t a1 = (seq_t)3;   /* a(1)   */
    seq_t a2 = (seq_t)14;  /* a(2)   */
    seq_t a3 = (seq_t)17;  /* a(3)   */

    for (idx_t i = 4; i <= n; ++i)
    {
        /* a(i) = 6*a(i-2) - a(i-4) */
        seq_t ai = (seq_t)6 * a2 - a0;

        /* shift the window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }

    /* a3 holds a(n) after the loop */
    return a3;
}
