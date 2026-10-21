/*
 * A097448  If n is square, replace it with sqrt(n).
 * https://oeis.org/A097448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n if n is not a perfect square, otherwise sqrt(n)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(sqrt(n)) for 0 <= n <= UINT64_MAX.
   Uses only integer arithmetic and no external utilities. */
static inline idx_t
isqrt_u64(idx_t n)
{
    idx_t lo = 0;
    /* The square root of a 64‑bit integer does not exceed 2^32-1,
       so we can use 2^32 as an upper bound. */
    idx_t hi = ((idx_t)1) << 32;     /* 0x100000000ULL == 2^32 */

    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;           /* upper mid to avoid infinite loop */
        if ((unsigned __int128)mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    idx_t r = isqrt_u64(n);
    if ((unsigned __int128)r * r == n)           /* perfect square detected */
        return (seq_t)r;
    else
        return (seq_t)n;
}
