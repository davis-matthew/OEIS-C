/*
 * A042456  Numerators of continued fraction convergents to sqrt(756).
 * https://oeis.org/A042456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042456: a(n) = 110*a(n-2) - a(n-4)
   with a(0)=27, a(1)=55, a(2)=2997, a(3)=6049 */
seq_t a(idx_t n)
{
    /* Handle the first four explicit terms. */
    if (n == 0) return (seq_t)27;
    if (n == 1) return (seq_t)55;
    if (n == 2) return (seq_t)2997;
    if (n == 3) return (seq_t)6049;

    /* Rolling buffer for the last four values:
       a0 = a(i-4), a1 = a(i-3), a2 = a(i-2), a3 = a(i-1) */
    seq_t a0 = (seq_t)27;     /* a(0) */
    seq_t a1 = (seq_t)55;     /* a(1) */
    seq_t a2 = (seq_t)2997;   /* a(2) */
    seq_t a3 = (seq_t)6049;   /* a(3) */

    for (idx_t i = 4; i <= n; ++i)
    {
        /* Compute the recurrence value using only seq_t operations. */
        seq_t next = a2 * (seq_t)110;
        next -= a0;

        /* Slide the window forward for the next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;   /* a(i) */
    }

    /* After the loop, a3 holds a(n). */
    return a3;
}
