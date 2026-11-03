/*
 * A171746  Let f(n) = n + floor(sqrt(n)). Then a(n) is the smallest number of
 *          iterations of f on n such that a perfect square is obtained.
 * https://oeis.org/A171746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest k ≥ 1 such that f⁽ᵏ⁾(n) is a perfect *
 *            square, where f(m) = m + ⌊√m⌋. * This implementation applies f
 *            iteratively until a square is reached * and counts the number of
 *            iterations
 * Range:     u8 n <= 15, u16 n <= 7488, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 109 terms through n = 7488 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    seq_t x = (seq_t)n;   /* current value of the iteration */
    seq_t steps = 0;      /* number of f-iterations performed */

    do {
        /* perform one iteration of f */
        steps++;
        seq_t r = floor_sqrt(x);
        x = x + r;

    /* continue while the new value is not a perfect square */
    } while (floor_sqrt(x) * floor_sqrt(x) != x);

    return steps;
}
