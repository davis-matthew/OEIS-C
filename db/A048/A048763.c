/*
 * A048763  Smallest cube >= n.
 * https://oeis.org/A048763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest cube ≥ n, i.e. ceil(cuberoot(n))^3
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9609 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The helper floor_cube_root is declared in the utils library. */
#include <stdint.h>
#include "../utils/ipow_i32.h"

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

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    /* Convert the index to seq_t to perform operations in the correct type. */
    seq_t x = (seq_t)n;

    /* Find the greatest integer r such that r^3 ≤ x. */
    seq_t r = floor_cube_root(x);

    /* Compute r^3. */
    seq_t r3 = r * r * r;

    /* If r^3 is still less than x, increment r to get the next cube. */
    if (r3 < x) {
        r++;
        r3 = r * r * r;
    }

    return r3;
}
