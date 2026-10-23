/*
 * A041425  Denominators of continued fraction convergents to sqrt(228).
 * https://oeis.org/A041425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 302*a(n-2) - a(n-4) for n ≥ 4 with initial values a(0)=1,
 *            a(1)=10, a(2)=301, a(3)=3020
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the four known terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)301;
    if (n == 3) return (seq_t)3020;

    /* Rolling 4‑term buffer for the recurrence.
       a0 = a(i-4), a1 = a(i-3), a2 = a(i-2), a3 = a(i-1)   */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)10;  /* a(1) */
    seq_t a2 = (seq_t)301; /* a(2) */
    seq_t a3 = (seq_t)3020;/* a(3) */
    seq_t an;              /* current term */

    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)302 * a2 - a0;   /* a(i) = 302*a(i-2) - a(i-4) */

        /* Shift the window for the next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return an;
}
