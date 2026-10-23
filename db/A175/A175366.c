/*
 * A175366  Partial sums of A175365.
 * https://oeis.org/A175366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = |{(x,y,z) ∈ ℤ³ : |x|³+|y|³+|z|³ ≤ n, -n ≤ x,y,z ≤ n}| *
 *            The implementation counts non‑negative triples (x,y,z) and
 *            multiplies * by the number of sign choices for each coordinate
 * Range:     u8 n <= 42, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: integer cube root (floor) */
static idx_t integer_cuberoot(idx_t n)
{
    idx_t k = 0;
    while (1) {
        unsigned __int128 c = (unsigned __int128)(k + 1) * (k + 1) * (k + 1);
        if (c > n) break;
        ++k;
    }
    return k;
}

/* Actual sequence function */
seq_t a(idx_t n)
{
    idx_t max = integer_cuberoot(n);          /* maximum |x|, |y|, |z| needed */
    seq_t count = 0;

    for (idx_t x = 0; x <= max; ++x) {
        idx_t cx = (idx_t)((unsigned __int128)x * x * x);
        for (idx_t y = 0; y <= max; ++y) {
            idx_t cy = (idx_t)((unsigned __int128)y * y * y);
            idx_t sumxy = cx + cy;
            if (sumxy > n) break;
            idx_t rem = n - sumxy;
            idx_t zmax = integer_cuberoot(rem);

            /* weight for sign choices of x and y */
            seq_t wxy = 1;
            if (x != 0) wxy <<= 1;   /* multiply by 2 if x ≠ 0 */
            if (y != 0) wxy <<= 1;   /* multiply by 2 if y ≠ 0 */

            /* add contributions of all z from 0 to zmax */
            count += wxy * (seq_t)(1 + 2 * zmax);
        }
    }

    return count;
}
