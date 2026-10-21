/*
 * A235896  Number of (n+1) X (2+1) 0..2 arrays with the minimum plus the
 *          maximum equal to the lower median plus the upper median in every 2
 *          X 2 subblock.
 * https://oeis.org/A235896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1) - 44*a(n-2) - 24*a(n-3) + 192*a(n-4) +
 *            144*a(n-5) Initial values: a(1)=125, a(2)=641, a(3)=3265,
 *            a(4)=17001, a(5)=89857
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle offset */
    if (n < 1) return 0;  /* behaviour below offset is undefined */

    /* Return initial terms directly */
    if (n == 1) return 125;
    if (n == 2) return 641;
    if (n == 3) return 3265;
    if (n == 4) return 17001;
    if (n == 5) return 89857;

    /* Variables to hold the five previous terms:
        prev1 = a(k-1), prev2 = a(k-2), ..., prev5 = a(k-5)  */
    seq_t prev1 = 89857;   /* a(5) */
    seq_t prev2 = 17001;   /* a(4) */
    seq_t prev3 = 3265;    /* a(3) */
    seq_t prev4 = 641;     /* a(2) */
    seq_t prev5 = 125;     /* a(1) */
    seq_t curr = 0;

    for (idx_t i = 6; i <= n; ++i) {
        /* Compute a(i) from the five predecessors */
        curr = 13 * prev1 - 44 * prev2 - 24 * prev3 + 192 * prev4 + 144 * prev5;

        /* Shift the window for the next iteration */
        prev5 = prev4;
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}
