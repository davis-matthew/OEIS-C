/*
 * A035857  Coordination sequence for A_31 lattice.
 * https://oeis.org/A035857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d=1}^{31} C(32,d) * C(n-1,d-1) * C(n+31-d , n) (with
 *            a(0) = 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 12, u128 n <= 13,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;
    seq_t sum = 0;
    for (idx_t d = 1; d <= 31; ++d) {
        if (d-1 > n-1)      /* binomial will be zero for larger d */
            break;
        seq_t c32d = binomial((idx_t)32, d);
        seq_t cnd1 = binomial(n - 1, d - 1);
        seq_t cnd2 = binomial(n + 31 - d, n);
        seq_t term = c32d * cnd1;
        term = term * cnd2;
        sum = sum + term;
    }
    return sum;
}
