/*
 * A070091  Number of isosceles integer triangles with perimeter n and
 *          relatively prime side lengths.
 * https://oeis.org/A070091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of isosceles integer triangles with perimeter n and coprime side lengths.
 * Computation uses the range: floor(n/4)+1 ≤ b ≤ floor((n-1)/2), where b is the length of the two equal sides
 * and a = n - 2b is the third side.  A triangle is counted if gcd(a, b) == 1.
 */

#include <stdint.h>
#include "../utils/gcd_uint64.h"

/* Euclidean gcd for 64‑bit integers used in the search. */

seq_t a(idx_t n)
{
    /* For n < 3 there are no non‑degenerate isosceles triangles. */
    if (n < 3) return 0;

    seq_t count = 0;

    /* b is the length of the two equal sides.  Range derived from a > 0 and 4b > n. */
    idx_t b_min = n / 4 + 1;
    idx_t b_max = (n - 1) / 2;

    for (idx_t b = b_min; b <= b_max; ++b) {
        idx_t a_side = n - 2 * b;          /* the third side a = n - 2b */
        /* All sides fit in 64 bits for the given n constraints. */
        if (gcd_uint64((uint64_t)a_side, (uint64_t)b) == 1) {
            ++count;
        }
    }
    return count;
}
