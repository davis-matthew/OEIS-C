/*
 * A035808  Coordination sequence for lattice D*_46 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..46} 2^k*binomial(46,k)*binomial(n-1,k-1) +
 *            2^46*binomial((46+2*n)/2-1,45)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Special case: a(0) = 1 */
    if (n == 0)
        return 1;

    const idx_t N = 46;
    seq_t sum = 0;

    /* First sum: k = 1 .. N */
    for (idx_t k = 1; k <= N; ++k) {
        seq_t term = pow2(k) * binomial(N, k);
        term *= binomial(n - 1, k - 1);
        sum += term;
    }

    /* Second term */
    idx_t c = (N + 2 * n) / 2 - 1;   /* (N + 2*n)/2 - 1 */
    sum += pow2(N) * binomial(c, N - 1);

    return sum;
}
