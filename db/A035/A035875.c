/*
 * A035875  Coordination sequence for A_49 lattice.
 * https://oeis.org/A035875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n==0 a(n) = Σ_{d=1}^{49} C(50,d) * C(n-1,d-1) *
 *            C(49-d+n,n) for n≥1 (where C is the binomial coefficient)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t sum = 0;
    /* d cannot exceed either 49 or n, because C(n-1,d-1)=0 if d-1>n-1 */
    idx_t max_d = (n < 49) ? n : 49;
    for (idx_t d = 1; d <= max_d; ++d) {
        seq_t term = binomial(50, d);
        term *= binomial(n - 1, d - 1);
        term *= binomial(49 - d + n, n);
        sum += term;
    }
    return sum;
}
