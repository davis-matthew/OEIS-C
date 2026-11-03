/*
 * A140823  Integers that are not perfect fourth powers.
 * https://oeis.org/A140823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = nth positive integer that is not a perfect fourth power
 * Range:     u8 n <= 61, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  For example, 1, 16, 81, ... are excluded.  The sequence starts 2,3,4,...  */

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

static seq_t floor_fourth_root(seq_t x)
{
    /* floor fourth root of x : floor( sqrt( sqrt(x) ) ) */
    seq_t s = floor_sqrt(x);
    return floor_sqrt(s);
}

static bool is_fourth_power(seq_t x)
{
    seq_t r = floor_fourth_root(x);
    seq_t r2 = r * r;
    seq_t r4 = r2 * r2;
    return r4 == x;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (seq_t x = 1; ; ++x)
    {
        if (is_fourth_power(x))
            continue;
        ++count;
        if (count == (seq_t)n)
            return x;
    }
}
