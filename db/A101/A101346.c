/*
 * A101346  a(n) = binomial(2^n, n-1).
 * https://oeis.org/A101346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2^n, n-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   binom_big, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_big.h"
#include "../utils/pow2.h"

/* Compute a(n) = C(2^n, n-1) for n >= 1.
   Uses arbitrary-precision binomial calculation.
   seq_t and idx_t are supplied by the build. */
seq_t a(idx_t n) {
    /* 2^n */
    seq_t N = pow2(n);
    /* n-1 */
    seq_t k = (seq_t)(n - 1);
    return binom_big(N, k);
}
