/*
 * A085121  Number of ways of writing n as the sum of three odd squares.
 * https://oeis.org/A085121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of ways of writing n as the sum of three odd squares.
 *            a(n) = 8 * #{(x,y,z) | x,y,z odd > 0, x² + y² + z² = n}
 * Range:     u8 n <= 65, u16 n <= 9300, u32 n <= 9300, u64 n <= 9300,
 *            u128 n <= 9300, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm enumerates all odd x and y with x² + y² ≤ n, then
   checks whether the remaining sum is a perfect odd square. This is
   O(√n · √n) in the worst case, but for n ≤ 10 000 it runs in well under
   2 s and scales easily. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t count = 0;          /* number of positive-ordered solutions */

    for (idx_t x = 1; ; x += 2) {
        seq_t x_sq = (seq_t)x * (seq_t)x;
        if (x_sq > N) break;
        seq_t rem1 = N - x_sq;

        for (idx_t y = 1; ; y += 2) {
            seq_t y_sq = (seq_t)y * (seq_t)y;
            if (y_sq > rem1) break;
            seq_t rem2 = rem1 - y_sq;
            if (rem2 == 0) continue;            /* would give z = 0 */

            seq_t z = floor_sqrt(rem2);         /* integer part of sqrt(rem2) */
            if (z * z != rem2) continue;        /* not a perfect square */
            if ((z & 1u) == 0) continue;        /* z must be odd */

            ++count;                            /* one positive triple found */
        }
    }

    return count * (seq_t)8;                  /* each positive triple has 8 sign variations */
}
