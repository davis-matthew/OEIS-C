/*
 * A175396  Numbers whose sum of squares of digits is a square.
 * https://oeis.org/A175396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = nth number whose sum of squares of its decimal digits is
 *            a perfect square
 * Range:     u8 n <= 8, u16 n <= 2523, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9689 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"

/* Helper: sum of squares of decimal digits of a 64‑bit integer. */
static uint64_t digit_square_sum(uint64_t x)
{
    uint64_t sum = 0;
    while (x > 0) {
        uint64_t d = x % 10;
        sum += d * d;
        x /= 10;
    }
    return sum;
}

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

/* Test whether a 64‑bit sum is a perfect square */
static bool is_square_uint64(uint64_t sum)
{
    seq_t r = floor_sqrt((seq_t)sum);
    return (r * r) == (seq_t)sum;
}

/* Computes the n‑th term of A175396 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n = 0 – return 0 for safety. */
        return 0;
    }

    idx_t candidate = 0;   /* start from 0, a(1) = 0 */
    idx_t found    = 0;    /* number of qualifying terms found so far */

    for (;;) {
        uint64_t ss = digit_square_sum(candidate);
        if (is_square_uint64(ss)) {
            ++found;
            if (found == n) {
                return (seq_t)candidate;
            }
        }

        /* Prevent wrap‑around of the candidate counter (should not happen
           for the required range). */
        if (candidate == UINT64_MAX) {
            break;
        }
        ++candidate;
    }

    /* In case n was larger than can be represented (unlikely) */
    return 0;
}
