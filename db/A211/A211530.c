/*
 * A211530  Number of -1..1 arrays x(i) of n+1 elements i=1..n+1 with
 *          set{t,u,v in 0,1}((x[i+t]+x[j+u]+x[k+v])*(-1)^(t+u+v)) having two,
 *          three, four, five or six distinct values for every i,j,k<=n.
 * https://oeis.org/A211530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 16*a(n-3) + 11*a(n-4) + 19*a(n-5) - 16*a(n-6)
 *            - 6*a(n-7) + 4*a(n-8) Initial values: a(1)=8, a(2)=22, a(3)=56,
 *            a(4)=124, a(5)=270, a(6)=560, a(7)=1156, a(8)=2336
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values for n = 1..8 */
    static const seq_t base[8] = {
        (seq_t)8,  (seq_t)22, (seq_t)56, (seq_t)124,
        (seq_t)270, (seq_t)560, (seq_t)1156, (seq_t)2336
    };

    if (n <= 8 && n >= 1)
        return base[n-1];

    /* If n == 0 or negative, undefined; return 0 as placeholder */
    if (n < 1)
        return (seq_t)0;

    /* Use a sliding window of the last 8 computed terms */
    seq_t win[8];
    int i;
    for (i = 0; i < 8; ++i)
        win[i] = base[i];

    /* Compute up to n */
    for (idx_t k = 9; k <= n; ++k) {
        /* win[7] = a(k-1), win[5] = a(k-3), win[4] = a(k-4),
           win[3] = a(k-5), win[2] = a(k-6), win[1] = a(k-7),
           win[0] = a(k-8) */
        seq_t next =
            (4 * win[7]) -
            (16 * win[5]) +
            (11 * win[4]) +
            (19 * win[3]) -
            (16 * win[2]) -
            (6 * win[1]) +
            (4 * win[0]);

        /* Shift window left by one */
        for (i = 0; i < 7; ++i)
            win[i] = win[i+1];
        win[7] = next;
    }
    return win[7];
}
