/*
 * A035827  Coordination sequence for lattice D*_84 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{84} 2^k * binom(84,k) * binom(n-1,k-1) + 2^84
 *            * binom(n+41,83), with a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 13, u128 n <= 14,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    const idx_t N = 84;
    seq_t sum = 0;

    /* First summation term */
    for (idx_t k = 1; k <= N; ++k) {
        seq_t term = pow2(k) * binomial(N, k) * binomial(n - 1, k - 1);
        sum += term;
    }

    /* Second term */
    seq_t second = pow2(N) * binomial(n + 41, 83);

    return sum + second;
}
