/*
 * A021544  Expansion of g.f. 1/((1 - x)*(1 - 3*x)*(1 - 7*x)*(1 - 8*x)).
 * https://oeis.org/A021544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19*a(n-1)-119*a(n-2)+269*a(n-3)-168*a(n-4) with initial
 *            values: a(0)=1, a(1)=19, a(2)=242, a(3)=2606
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)19;
    if (n == 2) return (seq_t)242;
    if (n == 3) return (seq_t)2606;

    /* Sliding window for the recurrence */
    seq_t a0 = (seq_t)1;      /* a(n-4) */
    seq_t a1 = (seq_t)19;     /* a(n-3) */
    seq_t a2 = (seq_t)242;    /* a(n-2) */
    seq_t a3 = (seq_t)2606;   /* a(n-1) */
    seq_t an = 0;

    for (idx_t i = 4; i <= n; ++i) {
        an = 19 * a3 - 119 * a2 + 269 * a1 - 168 * a0;

        /* advance the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return an;
}
