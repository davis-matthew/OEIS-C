/*
 * A061606  Ooguri-Vafa invariants of disk domain wall degeneracies for brane
 *          I in the O(K) -> P^1 X P^1 geometry.
 * https://oeis.org/A061606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1) * Σ_{k=0..4} h(k) * binomial(n + 12 - k, 12) where
 *            h(k) = (1, 22, 67, 22, 1). The binomial coefficient is defined
 *            as C(m, 12). For m < 12 the coefficient is zero
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 13, u64 n <= 93, u128 n <= 200,
 *            big any n
 * Helpers:   binom_idx_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We compute the binomial multiplicatively to keep intermediate values
   small and avoid overflowing seq_t.  */

#include <stdint.h>
#include "../utils/binom_idx_k.h"

static const seq_t H[5] = {(seq_t)1, 22, 67, 22, 1};

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= 4; ++k) {
        /* m = n + 12 - k */
        idx_t m = n + 12 - k;
        seq_t bin = binom_idx_k(m, 12);
        sum += H[k] * bin;
    }
    return (seq_t)(n + 1) * sum;
}
