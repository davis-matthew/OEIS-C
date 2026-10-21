/*
 * A075185  One-fourth of fifth column of triangle A075181.
 * https://oeis.org/A075185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)! * S1(n+5, n+1) / 4 (S1 = unsigned Stirling number
 *            of the first kind)
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) for OEIS sequence A075185.
 * Uses a recurrence for unsigned Stirling numbers of the first kind
 * with difference 4: s(m, m-4).  The function runs in O(n) time. */
seq_t a(idx_t n)
{
    /* arrays for S(m, m-d) with d = 0..4, initially for m = 0 */
    seq_t diff[5] = {1, 0, 0, 0, 0};          /* diff[d] = s(m, m-d) for current m */
    seq_t fact = 0;                            /* fact = (m-4)! for current m (defined when m>=4) */

    idx_t target_m = n + 5;                    /* m = n + 5 */

    for (idx_t m = 1; m <= target_m; ++m)
    {
        /* Update Stirling numbers for this m.
         * For d = 4..1: new_s(m,m-d) = s(m-1,m-1-d) + (m-1)*s(m-1,m-d).  */
        for (int d = 4; d >= 1; --d)
            diff[d] = diff[d] + (seq_t)(m - 1) * diff[d - 1];
        /* s(m,m) is always 1 */
        diff[0] = 1;

        /* Update factorial (m-4)! from previous factorial */
        if (m >= 4)
        {
            if (m == 4)
                fact = 1;          /* 0! = 1 */
            else
                fact *= (seq_t)(m - 4);
        }

        if (m == target_m)
        {
            /* Compute result: (n+1)! * s(m,m-4) / 4  with m = n+5 */
            seq_t s_val = diff[4];           /* s(m, m-4) */

            /* Avoid overflow for standard integer types */
            if (sizeof(seq_t) <= 8)
            {
                __uint128_t temp = (__uint128_t)fact * (__uint128_t)s_val;
                temp /= 4;
                return (seq_t)temp;
            }
            else
            {
                seq_t res = fact * s_val / 4;
                return res;
            }
        }
    }

    /* Should never reach here */
    return (seq_t)0;
}
