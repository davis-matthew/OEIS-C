/*
 * A206772  Table T(n,k)=max{4*n+k-4,n+4*k-4} n, k > 0, read by antidiagonals.
 * https://oeis.org/A206772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 4*(t+1) + 3*max{t*(t+1)/2 - n, n - (t*t + 3*t + 4)/2},
 *            where t = floor((-1 + sqrt(8*n - 7)) / 2)
 * Range:     u8 n <= 2080, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A206772: Table T(n,k)=max{4*n+k-4, n+4*k-4} read by antidiagonals. */

#include <stdint.h>

static inline uint64_t _isqrt128(unsigned __int128 v)
{
    uint64_t low = 0, high = 0xFFFFFFFFFFFFFFFFULL;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 == v) return mid;
        if (mid2 < v)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* Compute t = floor((-1 + sqrt(8*n - 7)) / 2) */
    unsigned __int128 v = (unsigned __int128)8 * n - 7;
    uint64_t sqrtv = _isqrt128(v);
    uint64_t t = (sqrtv - 1) / 2;

    /* part1 = 4 * (t + 1) */
    seq_t part1 = (seq_t)4 * (seq_t)(t + 1);

    /* difference terms – use signed 128 for safety */
    signed __int128 diff1 = (signed __int128)t * (t + 1) / 2 - (signed __int128)n;
    signed __int128 diff2 = (signed __int128)n - ( ( (signed __int128)t * t
                                                    + (signed __int128)3 * t
                                                    + 4 ) / 2 );
    signed __int128 maxdiff = diff1 > diff2 ? diff1 : diff2;

    /* final result */
    signed __int128 res = (signed __int128)part1 + (signed __int128)3 * maxdiff;
    return (seq_t)res;
}
