/*
 * A033749  Expansion of Product_{d | 37} theta_3(q^d).
 * https://oeis.org/A033749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions to i^2 + 37 j^2 = n (i,j ∈ ℤ)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt_uint16
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint16_t, if not already defined */
#include "../utils/isqrt_uint16.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t j = 0;; ++j) {
        idx_t j2 = j * j;
        idx_t prod = 37 * j2;
        if (prod > n) break;
        idx_t m = n - prod;                    /* m = n - 37 j^2 ≥ 0 */
        uint16_t sq = isqrt_uint16((uint16_t)m); /* sqrt of m, m ≤ n ≤ 10000 */
        if ((uint32_t)sq * (uint32_t)sq != (uint32_t)m) continue; /* not a square */

        seq_t rem = (m == 0) ? 1 : 2;          /* solutions for i: ±sqrt(m) and 0 */

        if (j == 0) {
            count += rem;
        } else {
            count += rem * 2;                  /* j and -j give distinct solutions */
        }
    }
    return count;
}
