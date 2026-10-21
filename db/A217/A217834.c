/*
 * A217834  Total number of solutions to the equation x^2+k*y^2 = n with x >
 *          0, y > 0, k > 0. (Order matters for the equation x^2+y^2 = n).
 * https://oeis.org/A217834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{n-1} #{(x,y): x^2 + k y^2 = n, x>0, y>0}
 * Range:     u8 n <= 64, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For k = 1 this counts every ordered pair (x,y) of positive integers
   with x^2 + y^2 = n. */
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Compute A217834(n) for all n ≥ 1 */
seq_t a(idx_t n)
{
    /* n is 1‑based. */
    if (n <= 1) return 0;

    seq_t total = 0;

    for (idx_t k = 1; k < n; ++k)
    {
        /* iterate over y until k*y^2 >= n */
        for (idx_t y = 1; ; ++y)
        {
            seq_t ky = (seq_t)k * y * y;      /* k · y² */
            if (ky >= (seq_t)n) break;       /* remaining x² would be ≤ 0 */

            seq_t rem = (seq_t)n - ky;       /* x² must equal rem > 0 */
            seq_t r   = floor_sqrt(rem);     /* integer part of √rem */

            if (r * r == rem)                /* rem is a perfect square */
                total += 1;                  /* one ordered solution (x,y,k) */
        }
    }

    return total;
}
