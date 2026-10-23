/*
 * A188202  Central coefficients in (1 + 2^n*x + x^2)^n.
 * https://oeis.org/A188202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n,k)*binomial(n-k, n-2k) *
 *            2^( n*(n-2k) )
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* special case for n = 0 */
    if (n == 0) return 1;

    seq_t sum = 0;
    idx_t half = n / 2;
    for (idx_t k = 0; k <= half; ++k) {
        seq_t c1 = binomial(n, k);
        seq_t c2 = binomial(n - k, n - 2 * k);
        idx_t exp = n * (n - 2 * k);          /* exponent fits in 64‑bit */
        seq_t p2  = pow2(exp);
        sum += c1 * c2 * p2;
    }
    return sum;
}
