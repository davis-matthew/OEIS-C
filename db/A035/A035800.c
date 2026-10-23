/*
 * A035800  Coordination sequence for lattice D*_30 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1}^{30} 2^k * binomial(30,k) * binomial(n-1,k-1) +
 *            2^30 * binomial(n+14,29) (a(0)=1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    const idx_t N = 30;
    static int initialized = 0;
    static seq_t coeff[31];

    if (!initialized) {
        for (idx_t k = 0; k <= N; ++k) {
            coeff[k] = pow2(k) * binomial(N, k);
        }
        initialized = 1;
    }

    seq_t sum = 0;
    for (idx_t k = 1; k <= N; ++k) {
        seq_t term = coeff[k];
        term = term * binomial(n - 1, k - 1);
        sum += term;
    }

    seq_t second = pow2(N) * binomial(n + 14, 29);
    return sum + second;
}
