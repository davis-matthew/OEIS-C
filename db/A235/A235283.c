/*
 * A235283  Number of (n+1) X (2+1) 0..3 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 4
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) - 15*a(n-3) + 4*a(n-4) * + 18*a(n-5)
 *            - 12*a(n-6) * with initial values * a(1)=40, a(2)=68, a(3)=104,
 *            a(4)=188, * a(5)=304, a(6)=572. * The function implements this
 *            recurrence directly, * computing each term from the preceding
 *            six and thus * avoiding any dynamic allocati...
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset 1 directly. */
    switch (n)
    {
        case 1: return (seq_t)40;
        case 2: return (seq_t)68;
        case 3: return (seq_t)104;
        case 4: return (seq_t)188;
        case 5: return (seq_t)304;
        case 6: return (seq_t)572;
    }

    /* For n >= 7 compute iteratively, keeping only the last six terms. */
    seq_t a1 = (seq_t)40;   /* a(n-6) */
    seq_t a2 = (seq_t)68;   /* a(n-5) */
    seq_t a3 = (seq_t)104;  /* a(n-4) */
    seq_t a4 = (seq_t)188;  /* a(n-3) */
    seq_t a5 = (seq_t)304;  /* a(n-2) */
    seq_t a6 = (seq_t)572;  /* a(n-1) */
    seq_t a7;               /* current term */

    for (idx_t i = 7; i <= n; ++i)
    {
        /* a(i) = 3*a(i-1) + 3*a(i-2) - 15*a(i-3) + 4*a(i-4)
         *        + 18*a(i-5) - 12*a(i-6) */
        a7 = a6 * 3 + a5 * 3 - a4 * 15 + a3 * 4 + a2 * 18 - a1 * 12;

        /* shift the window */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = a7;
    }

    return a6;  /* a(n) after the loop */
}
