/*
 * A212798  Row 3 of array in A212796.
 * https://oeis.org/A212798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n/3*(( (5+sqrt(21))/2 )^n + ( (5-sqrt(21))/2 )^n - 2)^2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalently a(n) = 3*n*A054493(n-1)^2, where u_n = r^n + s^n satisfies u_0=2, u_1=5,
   u_k = 5*u_{k-1} - u_{k-2} ) */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* offset starts at n=1 */

    /* compute u_n = r^n + s^n via linear recurrence */
    seq_t u_prev2 = (seq_t)2;            /* u_0 */
    seq_t u_prev1 = (seq_t)5;            /* u_1 */
    seq_t u_n;

    if (n == 0) u_n = u_prev2;
    else if (n == 1) u_n = u_prev1;
    else
    {
        idx_t i;
        for (i = 2; i <= n; ++i)
        {
            u_n = 5 * u_prev1 - u_prev2;   /* 5*u_{k-1} - u_{k-2} */
            u_prev2 = u_prev1;
            u_prev1 = u_n;
        }
    }

    /* diff = u_n - 2 */
    seq_t diff = u_n - (seq_t)2;

    /* result = n/3 * diff^2 */
    seq_t diff_sq = diff * diff;          /* intermediate product */
    seq_t n_seq  = (seq_t)n;
    seq_t result = (n_seq * diff_sq) / 3; /* division is exact */

    return result;
}
