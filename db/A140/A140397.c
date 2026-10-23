/*
 * A140397  a(n) = floor(3*phi*n) - 3*floor(phi*n) where phi = (1+sqrt(5))/2.
 * https://oeis.org/A140397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(3*phi*n) - 3*floor(phi*n) where phi = (1+sqrt(5))/2
 *            The implementation uses only integer arithmetic. floor(k*phi) is
 *            evaluated as (k + floor(k*sqrt(5)))/2, exploiting that
 *            floor(k*sqrt(5)) = floor(sqrt(5*k*k)). Similarly floor(3*k*phi)
 *            = (3*k + floor(sqrt(45*k*k)))/2
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* integer square root: floor(sqrt(x)) for x >= 0  */
static seq_t int_sqrt_seq(seq_t x)
{
    seq_t lo = 0;
    seq_t hi = x;
    while (lo < hi) {
        seq_t mid = (lo + hi + 1) >> 1;               /* midpoint, round up */
        /* avoid overflow in mid*mid by checking x / mid */
        if (mid > 0 && mid > x / mid) {
            hi = mid - 1;
        } else {
            if (mid * mid <= x)
                lo = mid;
            else
                hi = mid - 1;
        }
    }
    return lo;
}

seq_t a(idx_t n)
{
    seq_t kn = (seq_t)n;                     /* cast to seq_t once */
    /* floor(n*sqrt(5))  = floor(sqrt(5*n*n)) */
    seq_t sqrt5_n = int_sqrt_seq((seq_t)5 * kn * kn);
    seq_t fp      = (kn + sqrt5_n) / 2;      /* floor(n*phi)  */

    /* floor(3*n*sqrt(5)) = floor(sqrt(45*n*n)) */
    seq_t sqrt45_n = int_sqrt_seq((seq_t)45 * kn * kn);
    seq_t fp3      = (3 * kn + sqrt45_n) / 2;/* floor(3*n*phi) */

    return fp3 - 3 * fp;
}
