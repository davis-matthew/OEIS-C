/*
 * A047944  Number of 9-colorings of an n X n grid, up to rotational symmetry.
 * https://oeis.org/A047944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (9^(n²) + 2*9^((n² + 3*(n mod 2))/4) + 9^((n² + (n mod
 *            2))/2))/4
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* m = 9 */
    const seq_t m = (seq_t)9;

    /* Compute n² */
    seq_t n_sq = (seq_t)n * (seq_t)n;

    /* Term 1: 9^(n²) */
    seq_t term1 = big_pow(m, n_sq);

    /* n mod 2 (as seq_t) */
    seq_t mod2 = (seq_t)(n & 1);

    /* Term 2: 9^((n² + 3*(n mod 2))/4) */
    seq_t exp2 = (n_sq + 3 * mod2) / 4;
    seq_t term2 = big_pow(m, exp2);

    /* Term 3: 9^((n² + (n mod 2))/2) */
    seq_t exp3 = (n_sq + mod2) / 2;
    seq_t term3 = big_pow(m, exp3);

    /* Sum everything: term1 + 2*term2 + term3 */
    seq_t sum = term1 + 2 * term2 + term3;

    /* Divide by 4 to get the final result */
    return sum / 4;
}
