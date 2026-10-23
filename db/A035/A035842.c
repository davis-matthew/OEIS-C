/*
 * A035842  Coordination sequence for A_16 lattice.
 * https://oeis.org/A035842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1, for n = 0 a(n) = Σ_{d=1..16} C(17, d) * C(n-1, d-1) *
 *            C(16-d+n, n) for n ≥ 1 (C() denotes binomial coefficient)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t sum = 0;
    /* Only terms with d ≤ n contribute (C(n-1, d-1) = 0 otherwise) */
    for (idx_t d = 1; d <= 16 && d <= n; ++d) {
        seq_t t = binom(17, d);
        t *= binom(n - 1, d - 1);
        t *= binom(16 - d + n, n);
        sum += t;
    }
    return sum;
}
