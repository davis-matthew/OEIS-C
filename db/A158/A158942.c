/*
 * A158942  Nonsquares coprime to 10.
 * https://oeis.org/A158942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth positive integer that is coprime to 10 and not a
 *            perfect square. That is, odd numbers not divisible by 5 and not
 *            equal to a square
 * Range:     u8 n <= 23, u16 n <= 6043, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9905 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* not used (offset 1) */

    seq_t cur = 1;            /* start from 1 */
    idx_t count = 0;
    for (;;) {
        ++cur;

        /* skip even numbers (not coprime to 10) */
        if ((cur & 1) == 0)
            continue;

        /* skip multiples of 5 */
        if (cur % 5 == 0)
            continue;

        /* skip perfect squares */
        seq_t s = floor_sqrt(cur);
        if (s * s == cur)
            continue;

        /* this number satisfies the condition */
        ++count;
        if (count == n)
            return cur;
    }
}
