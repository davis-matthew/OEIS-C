/*
 * A212069  Number of (w,x,y,z) with all terms in {1,...,n} and 3*w = x+y+z.
 * https://oeis.org/A212069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of (w,x,y,z) with 1 ≤ w,x,y,z ≤ n and 3w = x+y+z.
 *            Computed by summing over w and counting the bounded solutions
 *            x+y+z=3w via inclusion–exclusion
 * Range:     u8 n <= 9, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Total number of solutions */
    __int128 total = 0;

    for (idx_t w = 1; w <= n; ++w) {
        /* S = 3*w - 3 is the sum of the shifted variables (x-1)+(y-1)+(z-1) */
        idx_t S = 3 * (w - 1);

        __int128 cnt = 0;

        /* All non–negative solutions: C(S+2,2) */
        idx_t m = S + 2;
        cnt += (__int128)m * (m - 1) / 2;

        /* Subtract solutions where one variable exceeds n-1 */
        if (S >= n) {
            idx_t m1 = S - n + 2;
            cnt -= (__int128)3 * m1 * (m1 - 1) / 2;
        }

        /* Add back solutions where two variables exceed n-1 */
        if (S >= 2 * n) {
            idx_t m2 = S - 2 * n + 2;
            cnt += (__int128)3 * m2 * (m2 - 1) / 2;
        }

        /* No term for all three variables exceeding n-1 because S < 3*n
           for all w ≤ n. */

        total += cnt;
    }

    return (seq_t)total;
}
