/*
 * A061605  Ooguri-Vafa invariants of disk domain wall degeneracies for brane
 *          I in the O(K) -> P^1 X P^1 geometry.
 * https://oeis.org/A061605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*(binomial(n+8,8)+6*binomial(n+7,8)+binomial(n+6,8))
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute the three binomial coefficients */
    seq_t c1 = binomial(n + 8, 8);
    seq_t c2 = binomial(n + 7, 8);
    seq_t c3 = binomial(n + 6, 8);

    /* Combine them according to the formula, taking care of the multiplier 6 */
    seq_t sum = c1 + c3 + c2 * (seq_t)6;

    /* Multiply by (n+1) using seq_t arithmetic */
    return (seq_t)(n + 1) * sum;
}
