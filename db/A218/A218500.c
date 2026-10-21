/*
 * A218500  8th iteration of the hyperbinomial transform on the sequence of
 *          1's.
 * https://oeis.org/A218500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + ∑_{k=1..n} 8 * (k+8)^(k-1) * C(n,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binom.h"

/* Compute a(n) where a(0) = 1,  a(n) = Σ_{k=0..n} 8*(k+8)^(k-1)*C(n,k),
 * with the convention that the term for k = 0 equals 1.
 */
seq_t a(idx_t n)
{
    /* For n = 0 the sum reduces to the single term k = 0. */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = (seq_t)1;   /* k = 0 term */

    for (idx_t k = 1; k <= n; ++k) {
        /* base = k + 8, exponent = k - 1 */
        seq_t base = (seq_t)(k + 8);
        seq_t exp  = (seq_t)(k - 1);

        /* pow_val = (k+8)^(k-1) */
        seq_t pow_val = big_pow(base, exp);

        /* bin = C(n,k) */
        seq_t bin = binom(n, k);

        /* term = 8 * pow_val * bin */
        seq_t term = pow_val * bin * (seq_t)8;

        sum += term;
    }

    return sum;
}
