/*
 * A194195  First inverse function (numbers of rows) for pairing function
 *          A060734.
 * https://oeis.org/A194195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = min{t; t^2 - n + 1}, where t = floor(sqrt(n-1)) + 1
 * Range:     u8 n <= 9870, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root (floor) for a 64‑bit unsigned integer. */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    if (n <= 1)
        return n;

    idx_t lo = 0, hi = n >> 1;
    if (hi + 1 < hi)       /* hi == UINT64_MAX / 2, +1 cannot overflow */
        hi = n;            /* fallback for huge n */

    while (lo <= hi)
    {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq == (unsigned __int128)n)
            return mid;
        if (sq < (unsigned __int128)n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* n >= 1 by specification – the caller must not call with n = 0 */
    idx_t t = floor_sqrt_u64(n - 1) + 1;          /* t = floor(sqrt(n-1)) + 1 */

    /* Compute t^2 and the candidate value t^2 - n + 1 using 128‑bit
       arithmetic to avoid overflow for any 64‑bit index. */
    unsigned __int128 t128   = (unsigned __int128)t;
    unsigned __int128 cand   = t128 * t128 - (unsigned __int128)n + 1;
    unsigned __int128 result = (t128 <= cand) ? t128 : cand;

    return (seq_t)result;                          /* cast to seq_t type */
}
