/*
 * A042420  Numerators of continued fraction convergents to sqrt(738).
 * https://oeis.org/A042420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 326*a(n-2) - a(n-4) a(0) = 27, a(1) = 163, a(2) = 8829,
 *            a(3) = 53137 (OEIS A042420)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly. */
    if (n == 0) return (seq_t)27;
    if (n == 1) return (seq_t)163;
    if (n == 2) return (seq_t)8829;
    if (n == 3) return (seq_t)53137;

    /* Compute from n = 4 up to the requested index.  We keep a sliding
       window of the last four values for the recurrence. */
    seq_t a0 = (seq_t)27;      /* a(0)   */
    seq_t a1 = (seq_t)163;     /* a(1)   */
    seq_t a2 = (seq_t)8829;    /* a(2)   */
    seq_t a3 = (seq_t)53137;   /* a(3)   */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i)
    {
        an = (seq_t)326 * a2 - a0;  /* a(i) = 326*a(i-2) - a(i-4) */

        /* Slide the window: discard a0, shift others. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return an; /* when i == n, an holds a(n) */
}
