/*
 * A072976  a(n) = 2^n*binomial(3*n,n)*(3*n+2).
 * https://oeis.org/A072976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n * binomial(3*n, n) * (3*n + 2)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t three_n = 3 * n;
    /* Compute C(3n, n) */
    seq_t bin = binomial(three_n, n);

    /* Compute 2^n */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 *= 2;
    }

    /* Multiply the components together */
    seq_t res = pow2 * bin;
    res *= (seq_t)(3 * n + 2);

    return res;
}
