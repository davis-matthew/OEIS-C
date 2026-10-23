/*
 * A171919  Number of solutions to n=x*y*z, x+y-z=1 with ordered triples
 *          (x,y,z), x,y,z>=1.
 * https://oeis.org/A171919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (x,y,z ≥ 1) with n = x*y*z and
 *            x + y – z = 1
 * Range:     u8 n <= 15, u16 n <= 9598, u32 n <= 9598, u64 n <= 9598,
 *            u128 n <= 9598, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for seq_t (floor(sqrt(n))) */
static seq_t isqrt_seq(seq_t n)
{
    seq_t low  = 0;
    seq_t high = n;
    while (low <= high) {
        seq_t mid = low + ((high - low) >> 1);
        seq_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset starts at 1 */
    seq_t count = 0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;

        /* first divisor x = d */
        {
            idx_t x = d;
            seq_t t = (seq_t)n / x;
            seq_t x_minus1 = (seq_t)(x - 1);
            seq_t D = x_minus1 * x_minus1 + 4 * t;
            seq_t sqrtD = isqrt_seq(D);
            if (sqrtD * sqrtD == D) {
                seq_t diff = sqrtD - x_minus1;
                if ((diff & 1) == 0) {
                    seq_t y = diff >> 1;
                    if (y > 0) count++;
                }
            }
        }

        /* second divisor x = n/d, distinct from d */
        if (d != n / d) {
            idx_t x = n / d;
            seq_t t = (seq_t)n / x;
            seq_t x_minus1 = (seq_t)(x - 1);
            seq_t D = x_minus1 * x_minus1 + 4 * t;
            seq_t sqrtD = isqrt_seq(D);
            if (sqrtD * sqrtD == D) {
                seq_t diff = sqrtD - x_minus1;
                if ((diff & 1) == 0) {
                    seq_t y = diff >> 1;
                    if (y > 0) count++;
                }
            }
        }
    }

    return count;
}
