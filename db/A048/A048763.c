/*
 * A048763  Smallest cube >= n.
 * https://oeis.org/A048763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest cube ≥ n, i.e. ceil(cuberoot(n))^3
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The helper floor_cube_root is declared in the utils library. */
#include "../utils/floor_cube_root.h"

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
