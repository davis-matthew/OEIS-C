/*
 * A081643  Integers congruent to 0, 1, 4 or 9 (mod 16) which are not squares.
 * https://oeis.org/A081643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 989 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A081643:
   a(n) = the n-th positive integer (n≥1) that is congruent to 0, 1, 4, or 9 (mod 16)
   and is NOT a perfect square. */

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

/* Test whether x is a perfect square using the provided floor_sqrt routine. */
static bool is_square(seq_t x)
{
    seq_t r = floor_sqrt(x);
    return (r * r == x);
}

seq_t a(idx_t n)
{
    idx_t count = 0;        /* how many qualifying numbers we have seen */
    seq_t m = 1;            /* candidate integer */

    for (;; ++m) {
        /* Keep only numbers ≡ 0,1,4,9 (mod 16).  The lowest 4 bits hold the residue. */
        seq_t r = m & 15;
        if (r == 0 || r == 1 || r == 4 || r == 9) {
            /* Exclude perfect squares. */
            if (!is_square(m)) {
                ++count;
                if (count == n)
                    return m;
            }
        }
    }
}
