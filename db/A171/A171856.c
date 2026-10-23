/*
 * A171856  Number of n-step up-side self-avoiding walks on the lattice strip
 *          {-1,0,1} x Z (up-side means that the walks move up and sideways
 *          but not down).
 * https://oeis.org/A171856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - a(n-2) + a(n-3) + a(n-4)   for n >= 4
   Initial values: a(0)=1, a(1)=3, a(2)=5, a(3)=9 */

seq_t a(idx_t n)
{
    /* Handle trivial cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)9;

    /* Use a rolling buffer of the last four terms */
    seq_t a0 = (seq_t)1;  /* a(n-4) */
    seq_t a1 = (seq_t)3;  /* a(n-3) */
    seq_t a2 = (seq_t)5;  /* a(n-2) */
    seq_t a3 = (seq_t)9;  /* a(n-1) */
    seq_t an = 0;

    for (idx_t i = 4; i <= n; ++i)
    {
        an = (seq_t)2 * a3 - a2 + a1 + a0;   /* recurrence */
        /* shift the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
