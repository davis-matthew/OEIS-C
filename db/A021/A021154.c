/*
 * A021154  Expansion of g.f. 1/((1 - x)*(1 - 2*x)*(1 - 5*x)*(1 - 11*x)).
 * https://oeis.org/A021154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=19, a(2)=256, a(3)=3066;
 *            a(n)=19*a(n-1)-105*a(n-2)+197*a(n-3)-110*a(n-4) for n>=4
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)19;
    if (n == 2) return (seq_t)256;
    if (n == 3) return (seq_t)3066;

    /* Initialise the first four terms */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)19;  /* a(1) */
    seq_t a2 = (seq_t)256; /* a(2) */
    seq_t a3 = (seq_t)3066;/* a(3) */
    seq_t a;               /* placeholder for a(n) */

    /* Compute up to n using the 4‑term recurrence */
    for (idx_t i = 4; i <= n; ++i) {
        a = 19 * a3 - 105 * a2 + 197 * a1 - 110 * a0;
        /* Shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a;
    }
    return a3; /* a(n) */
}
