/*
 * A089887  Number of subsets of {1,.., n} containing no squares.
 * https://oeis.org/A089887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^( n - floor(sqrt(n)) )
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t floor_sqrt_u64(idx_t n)
{
    if (n < 2) return n;
    idx_t low = 1, high = n >> 1, ans = 0;
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n)
        {
            ans = mid;
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* offset is 1; n==0 is not queried */
    idx_t sq  = floor_sqrt_u64(n);
    idx_t exp = n - sq;
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        result *= (seq_t)2;
    return result;
}
