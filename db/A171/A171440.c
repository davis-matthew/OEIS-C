/*
 * A171440  Expansion of (1+x)^5/(1-x).
 * https://oeis.org/A171440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{j=0..min(6,n), j≡n (mod2)} binomial(6,j)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   (which is equivalent to the expansion of (1+x)^5/(1-x)) */

#include <stdint.h>
#include "../utils/binomial.h"   /* provides binomial(idx_t, idx_t) */

static const seq_t binom6[7] = {1, 6, 15, 20, 15, 6, 1};

seq_t a(idx_t n)
{
    /* Find the largest j (≤6) that appears in the sum */
    idx_t maxj = n;
    if (maxj > 6) {
        maxj = 6;
    }

    /* Start at 0 for even n, at 1 for odd n */
    idx_t start = (n & 1) ? 1 : 0;

    seq_t sum = 0;
    for (idx_t j = start; j <= maxj; j += 2) {
        sum += binom6[j];
    }

    return sum;
}
