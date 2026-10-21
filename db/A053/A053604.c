/*
 * A053604  Number of ways to write n as an ordered sum of 3 nonzero
 *          triangular numbers.
 * https://oeis.org/A053604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples of non‑zero triangular numbers
 *            whose sum is n
 * Range:     u8 n <= 9, u16 n <= 1701, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static bool is_triangular(seq_t x)
{
    if (x == 0) return false;                      /* triangular numbers are >0 */
    seq_t D = (seq_t)8 * x + 1;                     /* discriminant of k^2 + k - 2x = 0 */
    seq_t s = floor_sqrt(D);
    return s * s == D && ((s - 1) % 2) == 0;        /* 8x+1 must be a perfect square
                                                      and (sqrt-1) even          */
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;                           /* minimal sum of 3 non‑zero triangles is 3 */

    seq_t count = 0;

    for (idx_t k1 = 1;; ++k1) {
        seq_t t1 = (seq_t)k1 * (seq_t)(k1 + 1) / 2;
        if (t1 > n) break;

        for (idx_t k2 = 1;; ++k2) {
            seq_t t2 = (seq_t)k2 * (seq_t)(k2 + 1) / 2;
            seq_t sum12 = t1 + t2;
            if (sum12 >= n) break;                 /* t3 would be <= 0 */

            seq_t t3 = (seq_t)n - sum12;
            if (is_triangular(t3)) count++;
        }
    }
    return count;
}
