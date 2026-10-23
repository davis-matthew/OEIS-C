/*
 * A176693  Union of squares and the even numbers.
 * https://oeis.org/A176693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 6785, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A176693: Union of squares and the even numbers.
   A(n) is the nth smallest integer k such that k is even or k is a perfect square.
   For any x >= 0, let
         evens(x)    = ⌊x/2⌋ + 1            (count of evens including 0)
         sq(x)       = ⌊√x⌋
         odd_squares = (sq(x) + 1)/2        (counts odd squares ≤ x)
   Then the number of elements of the set ≤ x is
         count(x) = evens(x) + odd_squares
   A(n) is the smallest x for which count(x) ≥ n.  This is found by binary search. */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

static seq_t count_upto(idx_t x)
{
    seq_t evens   = ((seq_t)x) / 2 + 1;
    seq_t sq      = floor_sqrt((seq_t)x);
    seq_t odd_sq  = (sq + 1) / 2;          /* odd squares up to x */
    return evens + odd_sq;
}

seq_t a(idx_t n)
{
    /* the sequence is non‑decreasing, so binary search works. */
    idx_t lo = 0, hi = 3 * n;              /* upper bound is safe for n ≤ 10000 */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((idx_t)count_upto(mid) >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return (seq_t)lo;
}
