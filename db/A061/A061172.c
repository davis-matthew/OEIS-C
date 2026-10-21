/*
 * A061172  Third column of Lucas bisection triangle (odd part).
 * https://oeis.org/A061172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A060924(n+2, 2) Recurrence: a(n) = 9*a(n-1) - 30*a(n-2) +
 *            45*a(n-3) - 30*a(n-4) + 9*a(n-5) - a(n-6) Initial terms: a(0)=9,
 *            a(1)=120, a(2)=753, a(3)=3612, a(4)=15040, a(5)=57366
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 14, u64 n <= 35, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle n < 6 directly */
    if (n == 0) return (seq_t)9;
    if (n == 1) return (seq_t)120;
    if (n == 2) return (seq_t)753;
    if (n == 3) return (seq_t)3612;
    if (n == 4) return (seq_t)15040;
    if (n == 5) return (seq_t)57366;

    /* initialise previous six terms */
    seq_t a0 = (seq_t)9;     /* a(n-6)  */
    seq_t a1 = (seq_t)120;   /* a(n-5)  */
    seq_t a2 = (seq_t)753;   /* a(n-4)  */
    seq_t a3 = (seq_t)3612;  /* a(n-3)  */
    seq_t a4 = (seq_t)15040; /* a(n-2)  */
    seq_t a5 = (seq_t)57366; /* a(n-1)  */
    seq_t next;

    for (idx_t i = 6; i <= n; ++i) {
        /* next = 9*a5 - 30*a4 + 45*a3 - 30*a2 + 9*a1 - a0
           rearranged to keep all intermediate values non–negative */
        seq_t t1 = 9 * a5 + 45 * a3 + 9 * a1;
        seq_t t2 = 30 * a4 + 30 * a2 + a0;
        next = t1 - t2;

        /* shift the window */
        a0 = a1; a1 = a2; a2 = a3; a3 = a4; a4 = a5; a5 = next;
    }
    return a5;
}
