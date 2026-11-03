/*
 * A212410  Let f(n) = n + floor(log(n)). Then a(n) is the smallest number of
 *          iterations of f on n such that the perfect square is obtained, or
 *          0 if no such square exists.
 * https://oeis.org/A212410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   * a(n) = smallest k >= 1 such that applying f(x) = x +
 *            floor(log_e(x)) * k times to n yields a perfect square. * Return
 *            0 if this never happens (which occurs only for n = 2). * * Note:
 *            log is natural log (base e). For integer n the floor is an *
 *            integer. The sequence has offset 1 (a(1) is the fir...
 * Range:     u8 n <= 22, u16 n <= 3517, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 95 terms through n = 8450 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
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

static inline uint64_t log_floor_uint64(uint64_t v)
{
    /* log() uses natural logarithm.  The result is within a few
     * ulps of the true value for all uint64_t, so floor() is exact.  */
    return (uint64_t)floor(log((double)v));
}

/* Check if x is a perfect square using floor_sqrt from utils.  */
static inline int is_square(seq_t x)
{
    seq_t s = floor_sqrt(x);
    return s * s == x;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    uint64_t cur = (uint64_t)n;   /* cur fits in uint64_t for the interval we need */
    seq_t steps = 0;

    while (1) {
        uint64_t inc = log_floor_uint64(cur);
        cur += inc;
        steps++;

        if (is_square((seq_t)cur))
            return steps;          /* square found after 'steps' iterations */

        if (inc == 0)
            return 0;              /* stuck in a cycle without reaching a square */
    }
}
