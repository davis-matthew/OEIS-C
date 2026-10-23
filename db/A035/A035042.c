/*
 * A035042  a(n) = 2^n - C(n,0)- ... - C(n,9).
 * https://oeis.org/A035042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - Σ_{k=0}^{9} C(n,k)
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the utilities that provide the necessary functions. */
#include "../utils/pow2.h"
#include "../utils/binomial.h"

/* Compute OEIS sequence A035042. */
seq_t a(idx_t n)
{
    /* For n < 10 the sum of the first ten binomial coefficients equals 2^n,
       so the sequence is zero. */
    if (n < 10) {
        return 0;
    }

    seq_t val = pow2(n);            /* 2ⁿ  */
    for (idx_t k = 0; k <= 9; ++k) { /* subtract C(n, k) for k=0..9 */
        val -= binomial(n, k);
    }
    return val;
}
