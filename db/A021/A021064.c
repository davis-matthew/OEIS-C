/*
 * A021064  Expansion of 1/((1-x)*(1-2*x)*(1-3*x)*(1-12*x)).
 * https://oeis.org/A021064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12^(n+3) - 55*3^(n+3) + 99*2^(n+3) - 45)/990 Recurrence:
 *            a(0)=1, a(1)=18, a(2)=241, a(3)=2982; for n ≥ 4: a(n) =
 *            18*a(n-1) - 83*a(n-2) + 138*a(n-3) - 72*a(n-4)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly. */
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)18;
    if (n == 2U) return (seq_t)241;
    if (n == 3U) return (seq_t)2982;

    /* Shifted window: a0 = a(n-4), a1 = a(n-3), a2 = a(n-2), a3 = a(n-1) */
    seq_t a0 = (seq_t)1;      /* a(0) */
    seq_t a1 = (seq_t)18;     /* a(1) */
    seq_t a2 = (seq_t)241;    /* a(2) */
    seq_t a3 = (seq_t)2982;   /* a(3) */
    seq_t an = 0;

    for (idx_t i = 4U; i <= n; ++i)
    {
        an = (seq_t)18 * a3
            - (seq_t)83 * a2
            + (seq_t)138 * a1
            - (seq_t)72 * a0;

        /* Shift window for next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
