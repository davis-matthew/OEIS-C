/*
 * A036711  a(n)=number of Gaussian integers z=a+bi satisfying |z|<=n+1/2,
 *          a>0, b>=0.
 * https://oeis.org/A036711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A036711: a(n) = number of Gaussian integers z = a + bi  
 * satisfying |z| <= n + 1/2, a > 0, b >= 0 
 */

#include <stdbool.h>   /* for bool, required by some utilities */
#include <stdint.h>    /* for uint64_t, used in idx_t cast */

static seq_t isqrt_seq(seq_t x)
{
    /* Integer square root: floor(sqrt(x)) for unsigned seq_t */
    seq_t low = 0;
    seq_t high = x;
    seq_t ans = 0;

    while (low <= high)
    {
        seq_t mid = (low + high) >> 1;     /* mid = (low+high)/2 */
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
        {
            ans = mid;                     /* possible answer */
            low = mid + 1;
        }
        else
        {
            if (mid == 0) break;           /* avoid underflow of high */
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* use 4R^2 = (2n+1)^2 to avoid fractions */
    seq_t two_n_plus_one = (seq_t)(2 * n + 1);
    seq_t limit_sq = two_n_plus_one * two_n_plus_one;

    seq_t count = 0;

    for (idx_t a = 1; a <= n; ++a)
    {   /* 4*a^2 */
        seq_t a_four_sq = ((seq_t)a * a) * 4;
        if (a_four_sq > limit_sq) break;   /* D4 would be negative */

        seq_t d4 = limit_sq - a_four_sq;   /* 4R^2 - 4a^2 = 4b^2 + remainder */
        seq_t d = isqrt_seq(d4);           /* floor(sqrt(4b^2 + rem)) */
        seq_t bmax = d / 2;                /* floor(b) */

        count += bmax + 1;                 /* b from 0 to bmax inclusive */
    }

    return count;
}
