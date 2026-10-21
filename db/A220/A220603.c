/*
 * A220603  First inverse function (numbers of rows) for pairing function
 *          A081344.
 * https://oeis.org/A220603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = mod(t;2)*min{t; n - (t - 1)^2} + mod(t + 1;2)*min{t; t^2
 *            - n + 1}, where t = floor[sqrt(n-1)] + 1
 * Range:     u8 n <= 9870, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   min_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/min_seq_t.h"

/* integer sqrt for uint64_t, returns floor(sqrt(n)) */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = 4294967296ULL; /* 2^32 */
    while (lo < hi)
    {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (!n)  /* should never happen because offset is 1, but guard anyway */
        return (seq_t)0;

    /* compute t = floor(sqrt(n-1)) + 1 */
    idx_t t = isqrt_u64(n - 1) + 1;

    seq_t part1 = 0, part2 = 0;

    /* part1 is used when t is odd */
    if ((t & 1ULL) != 0)    /* t % 2 == 1 */
    {
        unsigned __int128 u = (unsigned __int128)(t - 1);
        unsigned __int128 sq = u * u;                     /* (t-1)^2 */
        unsigned __int128 s1 = (unsigned __int128)n - sq; /* n - (t-1)^2 */
        seq_t s1t = (seq_t)s1;                           /* s1 <= t <= 2^32 */
        part1 = min_seq_t((seq_t)t, s1t);
    }

    /* part2 is used when t is even (since (t+1)%2 == 1) */
    if (((t + 1) & 1ULL) != 0)   /* t % 2 == 0 */
    {
        unsigned __int128 t128 = (unsigned __int128)t;
        unsigned __int128 sq = t128 * t128;               /* t^2 */
        unsigned __int128 s2 = sq - (unsigned __int128)n + 1; /* t^2 - n + 1 */
        /* s2 <= 2t-1 <= 2^33, so fits into seq_t safely */
        seq_t s2t = (seq_t)s2;
        part2 = min_seq_t((seq_t)t, s2t);
    }

    return part1 + part2;
}
