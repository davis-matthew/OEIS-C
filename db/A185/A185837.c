/*
 * A185837  Number of (n+1) X 2 0..2 arrays with every 2 X 2 subblock trace
 *          equal to some horizontal or vertical neighbor 2 X 2 subblock
 *          trace.
 * https://oeis.org/A185837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 6*a(n-2) - 15*a(n-3) + 120*a(n-4) - 144*a(n-5)
 *            + 144*a(n-6), with offset 1: a(1)=0, a(2)=171, a(3)=405,
 *            a(4)=3249, a(5)=12609, a(6)=72225
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the value for any n ≥ 1; for n=0 it returns 0. */

seq_t a(idx_t n)
{
    /* Base values for n = 1 … 6 */
    static const seq_t base[7] = {0, 0, 171, 405, 3249, 12609, 72225};

    if (n == 0) return 0ULL;                     /* safety for non‑offset calls */

    if (n <= 6) return base[n];                  /* quick return for small n */

    /* We only need the six most recent values.  Use a circular buffer. */
    seq_t a1 = base[6];    /* a(n-1)  when n = 7 */
    seq_t a2 = base[5];    /* a(n-2)  */
    seq_t a3 = base[4];    /* a(n-3)  */
    seq_t a4 = base[3];    /* a(n-4)  */
    seq_t a5 = base[2];    /* a(n-5)  */
    seq_t a6 = base[1];    /* a(n-6)  */

    /* Iterate from i = 7 up to n */
    for (idx_t i = 7; i <= n; ++i)
    {
        /* recurrence: a_i = 6*a1 - 6*a2 - 15*a3 + 120*a4 - 144*a5 + 144*a6 */
        seq_t ai =  6 * a1
                  - 6 * a2
                  - 15 * a3
                  + 120 * a4
                  - 144 * a5
                  + 144 * a6;

        /* shift the window: a6 becomes a5, a5 -> a4, …, a2 -> a1 */
        a6 = a5;
        a5 = a4;
        a4 = a3;
        a3 = a2;
        a2 = a1;
        a1 = ai;        /* new value becomes the newest term */
    }

    return a1;   /* a(n) is now stored in a1 */
}
