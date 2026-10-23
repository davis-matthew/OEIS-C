/*
 * A038665  Convolution of A007054 (super ballot numbers) with A000984
 *          (central binomial coefficients).
 * https://oeis.org/A038665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+3)*C(n+1) where C(k) is the k-th Catalan number
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* Catalan number C(n+1) */
    seq_t cat = catalan(n + 1);

    /* Multiply by (n+3) */
    cat *= (seq_t)(n + 3);

    return cat;
}
