/*
 * A162630  Triangle read by rows in which row n lists the number of states of
 *          the subshells of the n-th shell of the nuclear shell model ordered
 *          by energy level in increasing order.
 * https://oeis.org/A162630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2*(|2*n - t*t - 2*t - 3| + floor((2*n - t*t - t)/(t+3)))
 *            where t = floor((-1 + sqrt(8*n-7))/2)
 * Range:     u8 n <= 32, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for any unsigned integer type seq_t */
static seq_t sqrt_seq(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    seq_t mid, ans = 0;

    while (low <= high) {
        mid = low + (high - low) / 2;
        if (mid == 0) {
            ans = 0;
            low = 1;
            continue;
        }
        if (mid <= x / mid) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* n >= 1; behaviour for n==0 is undefined per OEIS offset */
    seq_t N = (seq_t)n;

    /* t = floor((-1 + sqrt(8*n-7))/2) */
    seq_t x = 8 * N - (seq_t)7;
    seq_t sqrt_val = sqrt_seq(x);
    seq_t t = (sqrt_val - (seq_t)1) / (seq_t)2;

    /* t^2 */
    seq_t t2 = t * t;

    /* |2*N - t^2 - 2*t - 3| */
    seq_t left1 = (seq_t)2 * N;
    seq_t right1 = t2 + (seq_t)2 * t + (seq_t)3;
    seq_t abs_part1 = (left1 >= right1) ? (left1 - right1) : (right1 - left1);

    /* floor((2*N - t^2 - t) / (t+3)) */
    seq_t left2 = (seq_t)2 * N;
    seq_t right2 = t2 + t;
    seq_t numerator = (left2 >= right2) ? (left2 - right2) : (right2 - left2);
    seq_t denominator = t + (seq_t)3;
    seq_t floor_part2 = numerator / denominator;

    /* Final result */
    seq_t result = (seq_t)2 * (abs_part1 + floor_part2);
    return result;
}
