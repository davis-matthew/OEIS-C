/*
 * A163104  Number of reduced words of length n in Coxeter group on 18
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A163104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence * a(0)=1, a(1)=18,
 *            a(2)=306, a(3)=5202, a(4)=88281, * a(n) =
 *            16*a(n-1)+16*a(n-2)+16*a(n-3)-136*a(n-4) for n ≥ 5 * * The
 *            generating function is
 *            (t^4+2*t^3+2*t^2+2*t+1)/(136*t^4-16*t^3-16*t^2-16*t+1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the five initial values explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)18;
    if (n == 2) return (seq_t)306;
    if (n == 3) return (seq_t)5202;
    if (n == 4) return (seq_t)88281;

    /* sliding window for a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t prev[4];
    prev[0] = (seq_t)18;     /* a(1) */
    prev[1] = (seq_t)306;    /* a(2) */
    prev[2] = (seq_t)5202;   /* a(3) */
    prev[3] = (seq_t)88281;  /* a(4) */

    seq_t curr = 0;

    for (idx_t i = 5; i <= n; ++i)
    {
        /* a(i) = 16*prev[3] + 16*prev[2] + 16*prev[1] - 136*prev[0] */
        seq_t sum = (seq_t)16 * prev[3] + (seq_t)16 * prev[2] + (seq_t)16 * prev[1];
        curr = sum - (seq_t)136 * prev[0];

        /* shift the window for the next iteration */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = curr;
    }

    return curr;
}
