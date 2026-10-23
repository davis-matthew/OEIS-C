/*
 * A016314  Expansion of 1/((1-2x)*(1-7x)*(1-11x)).
 * https://oeis.org/A016314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) - 113*a(n-2) + 154*a(n-3); Initial values:
 *            a(0)=1, a(1)=20, a(2)=287
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)20;
    if (n == 2) return (seq_t)287;

    /* Iteratively apply the recurrence */
    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)20;  /* a(n-2) */
    seq_t a2 = (seq_t)287; /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i)
    {
        /* a(i) = 20*a(i-1) - 113*a(i-2) + 154*a(i-3) */
        seq_t ai = (seq_t)20 * a2 - (seq_t)113 * a1 + (seq_t)154 * a0;

        /* Shift values for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }

    return a2;
}
