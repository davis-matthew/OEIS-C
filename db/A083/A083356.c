/*
 * A083356  Total area of all incongruent integer-sided rectangles of area <=
 *          n.
 * https://oeis.org/A083356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{r=1..⌊√n⌋} r·(r+⌊n/r⌋)·(⌊n/r⌋+1−r)/2
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the total area of all incongruent integer‑sided rectangles
   of area ≤ n.  The implementation uses only integer arithmetic and
   runs in O(√n) time.  It relies on the types idx_t and seq_t
   provided by the build system. */

#include <stdint.h>

/* Integer square root: floor(sqrt(n)) for 64‑bit n */
static idx_t floor_sqrt_u64(idx_t n)
{
    idx_t low = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t m = floor_sqrt_u64(n);
    for (idx_t r = 1; r <= m; ++r) {
        idx_t t = n / r;                        /* t = ⌊n/r⌋ */
        seq_t term = (seq_t)r;
        term = term * ((seq_t)(r + t));
        term = term * ((seq_t)(t + 1 - r));
        term = term / 2;
        sum += term;
    }
    return sum;
}
