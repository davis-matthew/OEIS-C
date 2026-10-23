/*
 * A035876  Coordination sequence for A_50 lattice.
 * https://oeis.org/A035876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d=1}^{50} C(51,d) * C(n-1,d-1) * C(50-d+n,n) (n >=
 *            1, a(0)=1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   binomial, mult_safe
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */
#include "../utils/mult_safe.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t sum = (seq_t)0;
    idx_t maxd = (n < 50) ? n : 50;

    for (idx_t d = 1; d <= maxd; ++d) {
        seq_t term1 = binomial((idx_t)51, d);
        seq_t term2 = binomial(n - 1, d - 1);
        seq_t term3 = binomial(50 - d + n, n);

        seq_t temp = mult_safe(term1, term2);
        temp = mult_safe(temp, term3);
        sum += temp;
    }

    return sum;
}
