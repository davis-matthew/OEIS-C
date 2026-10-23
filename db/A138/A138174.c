/*
 * A138174  a(n) is the smallest cube that begins with n^2.
 * https://oeis.org/A138174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: For a given n, let S = n².  For each number of
   appended decimal digits d = 0,1,…   compute the interval
   [S·10ᵈ , (S+1)·10ᵈ – 1].  The smallest cube in this interval
   (if any) is found by taking the integer cube root of the lower
   bound, rounding up, and cubing.  The first interval that
   contains a cube yields the answer.  This method is O(d) per
   n and produces the exact sequence. */

#include <stdint.h>

/* integer cube root: floor of the real cube root, using
   binary search on seq_t (works for arbitrary-precision). */
static inline seq_t int_floor_cuberoot(seq_t x)
{
    seq_t low = 0;
    seq_t high = 1;

    /* Expand high until high³ > x. */
    while (high * high * high <= x)
        high = high * 2;

    /* Binary search between low and high. */
    while (low + 1 < high) {
        seq_t mid = (low + high) / 2;
        seq_t cube = mid * mid * mid;
        if (cube <= x)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Base: S = n².  Compute as seq_t to avoid overflow. */
    seq_t S = (seq_t)n * (seq_t)n;

    seq_t ten = 1;                     /* 10ᵈ  (d = 0,1,…) */

    for (;;) {
        seq_t low  = S * ten;          /* interval lower bound    */
        seq_t high = (S + 1) * ten;     /* interval upper bound +1 */
        high = high - 1;

        /* Ceil cube root of low: floor(low‑1)³ + 1. */
        seq_t ceil_root = int_floor_cuberoot(low - 1) + 1;
        seq_t cube = ceil_root * ceil_root * ceil_root;

        if (cube <= high)
            return cube;               /* smallest cube in interval */

        /* Try next length d+1. */
        ten = ten * 10;
    }
}
