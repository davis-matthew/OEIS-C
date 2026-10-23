/*
 * A041067  Denominators of continued fraction convergents to sqrt(40).
 * https://oeis.org/A041067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 38 * a(n-2) - a(n-4) * with initial values a(0)=1,
 *            a(1)=3, a(2)=37, a(3)=114
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return the known initial values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)37;
    if (n == 3) return (seq_t)114;

    /* Variables hold a(n-4), a(n-3), a(n-2), a(n-1) respectively. */
    seq_t a0 = (seq_t)1,  /* a(i-4) */
          a1 = (seq_t)3,  /* a(i-3) */
          a2 = (seq_t)37, /* a(i-2) */
          a3 = (seq_t)114;/* a(i-1) */

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 38 * a(i-2) - a(i-4) */
        seq_t ai = (seq_t)38 * a2 - a0;

        /* Shift the window: a(i-3) becomes a(i-4), etc. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }

    return a3;
}
