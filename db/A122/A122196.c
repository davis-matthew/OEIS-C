/*
 * A122196  Fractal sequence: count down by 2's from successive integers.
 * https://oeis.org/A122196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = floor(sqrt(4*n-1)) - 2 * ((n-1) mod (t+1)), where t =
 *            floor((sqrt(4*n-3)-1)/2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t isqrt_u128(unsigned __int128 x)
{
    idx_t low = 0;
    idx_t high = (idx_t)~(idx_t)0;   /* maximum idx_t value */
    while (low <= high) {
        unsigned __int128 sum = (unsigned __int128)low + high;
        idx_t mid = (idx_t)(sum >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;   /* avoid underflow when mid=0 */
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    unsigned __int128 v1 = (unsigned __int128)4 * n - 1;
    unsigned __int128 v3 = (unsigned __int128)4 * n - 3;
    idx_t s1 = isqrt_u128(v1);
    idx_t s3 = isqrt_u128(v3);
    idx_t t = (s3 - 1) / 2;
    idx_t div = t + 1;
    idx_t r = (div == 0) ? 0 : (n - 1) % div;
    idx_t res = s1 - (idx_t)2 * r;
    return (seq_t)res;
}
