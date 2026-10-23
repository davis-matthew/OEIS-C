/*
 * A101688  Once 1, once 0, repeat, twice 1, twice 0, repeat, thrice 1, thrice
 *          0, ... and so on.
 * https://oeis.org/A101688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = floor(sqrt(2*n+1)) - floor(sqrt(2*n+1)-1/2)
 * Range:     u8 n <= 15, u16 n <= 2098, u32 n <= 9178, u64 n <= 9178,
 *            u128 n <= 9178, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t isqrt_seq(seq_t x)
{
    seq_t low = 0, high = x, mid, ans = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        seq_t sq = mid * mid;
        if (sq <= x) {
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
    seq_t x = (seq_t)n * 2 + 1;              /* 2*n + 1 */
    seq_t m = isqrt_seq(x);                 /* floor(sqrt(x)) */
    seq_t thresh = m * (m + 1) + 1;          /* m*(m+1)+1 */
    return (x < thresh) ? 1 : 0;             /* 1 if x < thresh else 0 */
}
