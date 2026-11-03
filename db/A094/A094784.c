/*
 * A094784  Numbers that are neither squares nor cubes.
 * https://oeis.org/A094784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9537 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A094784: Numbers that are neither squares nor cubes.
   The function searches for the nth such integer by testing each candidate
   and skipping perfect squares and cubes.  It uses the utility functions
   floor_sqrt and floor_cube_root which return the integer floor of the
   square/cube root.  The algorithm is O(a(n)), which is fast enough for
   n up to about 10000, as required. */
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

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static inline bool is_square(seq_t x) {
    seq_t s = floor_sqrt(x);
    return (s * s == x);
}

static inline bool is_cube(seq_t x) {
    seq_t r = floor_cube_root(x);
    return (r * r * r == x);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* Undefined for offset 0, return 0 */
    idx_t count = 0;
    seq_t val = 2;                 /* Candidates start at 2 */
    for (;;) {
        if (!is_square(val) && !is_cube(val)) {
            ++count;
            if (count == n) return val;
        }
        ++val;
    }
}
