/*
 * A175373  Partial sums of A175372.
 * https://oeis.org/A175373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(x,y) ∈ ℤ² | x⁴ + y⁴ ≤ n, -n ≤ x,y ≤ n} This counts all
 *            integer lattice points inside the fourth‑power circle.
 *            Implements an O( n^(1/4) ) algorithm using symmetry
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   floor_root4_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_root4_u64.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Max absolute coordinate that can satisfy x⁴ ≤ n. */
    idx_t M = floor_root4_u64(n);

    seq_t total = 0;

    for (idx_t x = 0; x <= M; ++x) {
        /* x⁴ fits in 64 bits because x ≤ 65536. */
        uint64_t x4 = (uint64_t)x * x * x * x;
        uint64_t rem = (uint64_t)n - x4;          /* remaining bound for y⁴ */

        idx_t R = floor_root4_u64(rem);          /* |y| ≤ R */
        uint64_t ycnt = 2 * R + 1;                /* number of y values */

        /* If x = 0, only one sign for x; otherwise double for ±x. */
        if (x == 0)
            total += (seq_t)ycnt;
        else
            total += (seq_t)2 * (seq_t)ycnt;
    }

    return total;
}
