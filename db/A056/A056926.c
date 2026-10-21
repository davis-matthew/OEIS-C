/*
 * A056926  a(n) = sqrt(n) if n is a square, otherwise 1.
 * https://oeis.org/A056926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sqrt(n) if n is a perfect square, otherwise 1. a(0) = 0.
 *            Uses a binary‑search integer square root
 * Range:     u8 n <= 9862, u16 n <= 9862, u32 n <= 9862, u64 n <= 9862,
 *            u128 n <= 9862, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    /* Special case: 0 is a square and its square root is 0. */
    if (n == 0)
        return (seq_t)0;

    uint64_t lo = 0;
    uint64_t hi = n;

    while (lo <= hi)
    {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;

        if (sq == n)
            return (seq_t)mid;                /* perfect square */
        else if (sq < n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }

    /* n is not a perfect square */
    return (seq_t)1;
}
