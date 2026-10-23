/*
 * A183920  Number of nondecreasing arrangements of n numbers in -2..2 with
 *          sum zero and sum of squares not greater than n*6/3.
 * https://oeis.org/A183920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nondecreasing arrangements of n numbers from
 *            -2..2 with sum zero and sum of squares <= 2*n. The following
 *            implementation directly counts the admissible multiplicity
 *            vectors (c2,c1,c0,c-1,c-2) using an O(n^2) enumerative routine
 * Range:     u8 n <= 21, u16 n <= 149, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t max_s = n / 2;         /* x + v ≤ n/2 by the square‑sum bound */

    for (idx_t s = 0; s <= max_s; ++s)
    {
        /* Upper bound for u + d from the square‑sum constraint:
           u + d ≤ min( n - 2*s , floor((n - s)/2) )  */
        idx_t B1 = n - 2 * s;
        idx_t B2 = (n - s) / 2;
        idx_t UB = (B1 < B2) ? B1 : B2;          /* integer upper bound */

        /* iterate d = v - x,  same parity as s, from -s to s */
        for (int64_t d = -(int64_t)s; d <= (int64_t)s; d += 2)
        {
            int64_t L = 0;
            if (d < 0)
                L = -2 * d;                     /* y = u + 2d ≥ 0  →  u ≥ -2d  */

            int64_t U = (int64_t)UB - d;        /* u + d ≤ UB       →  u ≤ UB - d */

            if (U >= L && L >= 0)
            {
                total += (seq_t)(U - L + 1);    /* count of admissible u values */
            }
        }
    }

    return total;
}
