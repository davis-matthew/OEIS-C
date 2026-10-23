/*
 * A022882  The number of numbers [ [ ix ]jx ] that equal n, where i >= 1, j
 *          >= 1 and x=sqrt(5).
 * https://oeis.org/A022882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of representations n = floor( sqrt(5) *
 *            floor(i * sqrt(5)) * j ) for integers i >= 1, j >= 1 and x =
 *            sqrt(5)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t
isqrt_u128(unsigned __int128 v)
{
    uint64_t lo = 0, hi = UINT64_MAX;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == v)
            return mid;
        if (sq < v)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* floor(m * sqrt(5))  using integer arithmetic:
 * floor(m * sqrt(5)) = floor( sqrt(5 * m^2) ). */
static uint64_t
floor_msqrt(uint64_t m)
{
    unsigned __int128 val = (unsigned __int128)5 * m * m;
    return isqrt_u128(val);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* find m such that floor(m*sqrt(5)) == n, if it exists */
    uint64_t lo = 1;
    uint64_t hi = (n + 1) / 2 + 1;   /* m ≤ (n+1)/sqrt(5) < (n+1)/2  */
    uint64_t m = 0;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        uint64_t val = floor_msqrt(mid);
        if (val < n)
            lo = mid + 1;
        else if (val > n)
            hi = mid - 1;
        else {
            m = mid;
            break;
        }
    }
    if (m == 0)
        return (seq_t)0;            /* no such m, answer is 0 */

    /* count all Beatty divisors k of m */
    uint64_t count = 0;
    for (uint64_t i = 1; ; ++i) {
        uint64_t k = floor_msqrt(i);     /* floor(i * sqrt(5)) */
        if (k > m)
            break;
        if (m % k == 0)
            ++count;
    }

    return (seq_t)count;
}
