/*
 * A035477  Coordination sequence for lattice D*_16 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..16} (2^k*binom(16,k)*binom(n-1,k-1)) +
 *            2^16*binom(n+7,15), with a(0)=1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 56, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    static bool init = false;
    static seq_t bin16[17];
    static seq_t p2[17];

    if (!init) {
        for (int k = 0; k <= 16; ++k) {
            bin16[k] = binomial(16, k);
            p2[k]   = pow2(k);
        }
        init = true;
    }

    seq_t total = 0;

    for (int k = 1; k <= 16; ++k) {
        seq_t term = p2[k] * bin16[k];
        term *= binomial(n - 1, k - 1);
        total += term;
    }

    total += p2[16] * binomial(n + 7, 15);

    return total;
}
