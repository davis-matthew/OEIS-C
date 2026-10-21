/*
 * A053442  Moments of generalized Motzkin paths.
 * https://oeis.org/A053442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 59, u128 n <= 116,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A053442: Moments of generalized Motzkin paths
   a(n) = ∑_{d≡n (mod 2), 0≤d≤⌊n/3⌋} (n−2d)! / (((n−3d)/2)!² d!)
        = ∑_{d≡n (mod 2)} C(n−2d, k) · C((n−d)/2, k),  where k = (n−3d)/2 */
#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    idx_t max_d = n / 3;           /* largest admissible d */
    idx_t start_d = (n & 1);       /* d must have same parity as n  */

    /* If no admissible d, the sum remains 0 (e.g. n = 1). */
    if (max_d < start_d)
        return sum;

    for (idx_t d = start_d; d <= max_d; d += 2) {
        /* k = (n-3d)/2, L = n-2d, m = (n-d)/2 */
        idx_t k = (n - 3 * d) / 2;
        idx_t L = n - 2 * d;
        idx_t m = (n - d) / 2;

        /* term = C(L, k) * C(m, k) */
        seq_t term = binom(L, k);
        term *= binom(m, k);

        sum += term;
    }

    return sum;
}
