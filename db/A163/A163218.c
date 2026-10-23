/*
 * A163218  Number of reduced words of length n in Coxeter group on 35
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A163218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=35, a(2)=1190, a(3)=40460, a(4)=1375045 and for
 *            n>=5 a(n) = 33*(a(n-1)+a(n-2)+a(n-3)) - 561*a(n-4) (recurrence
 *            from the G.F.)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values */
    const seq_t a0 = 1;
    const seq_t a1 = 35;
    const seq_t a2 = 1190;
    const seq_t a3 = 40460;
    const seq_t a4 = 1375045;

    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;
    if (n == 4) return a4;

    /* iterate using the recurrence */
    seq_t prev4 = a0; /* a(n-4)   */
    seq_t prev3 = a1; /* a(n-3)   */
    seq_t prev2 = a2; /* a(n-2)   */
    seq_t prev1 = a3; /* a(n-1)   */

    seq_t curr = 0;

    for (idx_t i = 4; i <= n; ++i)
    {
        /* compute a(i) */
        if (i == 4) {
            /* special term for n = 4: add the +1 from the numerator */
            curr = 33 * (prev1 + prev2 + prev3) - 561 * prev4 + 1;
        } else {
            curr = 33 * (prev1 + prev2 + prev3) - 561 * prev4;
        }

        /* shift the window */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
