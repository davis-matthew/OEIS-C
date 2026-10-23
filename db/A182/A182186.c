/*
 * A182186  Number b(n) of basic ideals in the Borel subalgebra of the
 *          untwisted affine Lie algebra of type B.
 * https://oeis.org/A182186
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n+5)*2^(2*n-2)-(2*(3*n-1))*binomial(2*n-2, n-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Return 0 for indices below the offset (n < 2) */
    if (n < 2) {
        return 0;
    }

    /* Calculate (3*n+5) * 2^(2*n-2) */
    seq_t term1 = (seq_t)(3 * n + 5) * pow2(2 * n - 2);

    /* Calculate (2*(3*n-1)) * binomial(2*n-2, n-1) */
    seq_t term2 = (seq_t)(2 * (3 * n - 1)) * binomial(2 * n - 2, n - 1);

    /* Subtract and return the result */
    return term1 - term2;
}
