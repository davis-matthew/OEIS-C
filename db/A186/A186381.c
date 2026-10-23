/*
 * A186381  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f(i)=4i and g(j)=j(j+1)/2 (triangular
 *          number). Complement of A186382.
 * https://oeis.org/A186381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor( ( sqrt(1 + 32*n) - 1 ) / 2 ) (OEIS A186381)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint64_t isqrt_u128(unsigned __int128 x)
{
    /* Fast integer square root for values up to 2^69  (sqrt <= 2^35). */
    uint64_t low = 0;
    uint64_t high = (uint64_t)1 << 35;          /* 2^35 > sqrt(2^69) */
    while (low <= high)
    {
        uint64_t mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
            low = mid + 1;
        else
        {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;                                /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    /* For safety below the offset; the value is irrelevant. */
    if (n == 0) return (seq_t)0;
    unsigned __int128 u = (unsigned __int128)1 + (unsigned __int128)32 * n;
    uint64_t s = isqrt_u128(u);                 /* floor(sqrt(1+32n)) */
    seq_t res = (seq_t)((unsigned __int128)n + (s - 1) / 2);  /* a(n) */
    return res;
}
