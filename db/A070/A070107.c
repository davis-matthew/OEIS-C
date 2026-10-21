/*
 * A070107  Number of integer triangles with perimeter n and relatively prime
 *          side lengths which are obtuse and isosceles.
 * https://oeis.org/A070107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer isosceles, obtuse triangles * with
 *            perimeter n and coprime side lengths. * For each integer a > 0
 *            let a be the equal sides, c = n−2a the base. * Conditions: n/4 <
 *            a < n/2, c > a, c² > 2a², gcd(a,c) = 1. * The function iterates
 *            over the limited range of a, tests the * cond...
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n <= 4) return 0;          /* no possible isosceles triangles */

    idx_t min_a = n / 4 + 1;       /* a > n/4   */
    idx_t max_a = (n - 1) / 2;     /* a < n/2   */
    seq_t count = 0;

    for (idx_t a_side = min_a; a_side <= max_a; ++a_side) {
        idx_t c_side = n - 2 * a_side;
        /* c_side > a_side already guaranteed by the bounds, but check anyway */
        if (c_side <= a_side) continue;

        /* obtain squares in 64‑bit to avoid overflow for all builds */
        uint64_t lhs = (uint64_t)c_side * (uint64_t)c_side;
        uint64_t rhs = 2ULL * (uint64_t)a_side * (uint64_t)a_side;
        if (lhs <= rhs) continue;      /* not obtuse */

        if (gcd_u64(a_side, c_side) != 1) continue;   /* not coprime */
        ++count;
    }

    return count;
}
