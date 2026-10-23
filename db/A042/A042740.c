/*
 * A042740  Numerators of continued fraction convergents to sqrt(901).
 * https://oeis.org/A042740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 60*a(n-1)+a(n-2); a(0)=30; a(1)=1801
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return the two base values directly. */
    if (n == 0) return 30;
    if (n == 1) return 1801;

    /* Iteratively compute the linear recurrence.  This works
       for any unsigned idx_t (up to ~10 000 is quick enough)
       and for any seq_t (built‑in or arbitrary‑precision). */
    seq_t a0 = 30;        /* a(n-2)   */
    seq_t a1 = 1801;      /* a(n-1)   */
    seq_t an;             /* a(n)     */

    for (idx_t i = 2; i <= n; ++i) {
        an   = 60 * a1 + a0;   /* recurrence */
        a0   = a1;             /* shift window */
        a1   = an;
    }
    return a1;
}
