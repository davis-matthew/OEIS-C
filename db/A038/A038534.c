/*
 * A038534  Numerators of coefficients of EllipticK/Pi.
 * https://oeis.org/A038534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = 2^(-2*w(n)) * binomial(2*n, n)^2 where w(n) is the number
 *            of 1‑bits in the binary expansion of n (A000120)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute binomial(2*n, n) */
    seq_t bin = binomial(n << 1, n);

    /* Count bits of n (w(n)) */
    unsigned int w = __builtin_popcountll((unsigned long long)n);

    /* Divide by 2^w(n); this division is exact */
    seq_t low = bin / pow2((idx_t)w);

    /* Square the result */
    return low * low;
}
