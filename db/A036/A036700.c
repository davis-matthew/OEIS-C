/*
 * A036700  Number of Gaussian integers z=a+bi satisfying |z|<=n, a>=0,
 *          0<=b<a.
 * https://oeis.org/A036700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Gaussian integers z = a + bi with |z| ≤ n, a ≥
 *            0, 0 ≤ b < a (equivalently, a(n) = Σ_{a=1}^{n} ( min(a-1, ⌊√(n²
 *            – a²)⌋) + 1 ) )
 * Range:     u8 n <= 8, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    seq_t n_sq = (seq_t)n * (seq_t)n;

    for (idx_t a_val = 1; a_val <= n; ++a_val) {
        seq_t a_sq = (seq_t)a_val * (seq_t)a_val;
        seq_t m    = n_sq - a_sq;          /* n² – a²  ≥ 0 for a ≤ n   */
        seq_t r    = floor_sqrt(m);        /* ⌊√(n² – a²)⌋           */

        seq_t a_minus_1 = (seq_t)a_val - 1;
        seq_t limit_b   = (a_minus_1 < r) ? a_minus_1 : r;
        total += limit_b + 1;              /* b ranges from 0 to limit_b */
    }

    return total;
}
