/*
 * A047940  Number of 5-colorings of an n X n grid, up to rotational symmetry.
 * https://oeis.org/A047940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5^(n^2) + 2*5^((n^2 + 3*(n mod 2))/4) + 5^((n^2 + (n mod
 *            2))/2))/4
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    /* reduce modulo 2 */
    idx_t n_mod2 = n & 1U;
    /* n^2 */
    idx_t n2 = n * n;
    /* exponents */
    idx_t exp1 = n2;
    idx_t exp2 = (n2 + 3 * n_mod2) >> 2;   /* (n^2 + 3*(n mod 2)) / 4 */
    idx_t exp3 = (n2 + n_mod2) >> 1;        /* (n^2 + (n mod 2)) / 2 */

    /* compute powers of 5 */
    seq_t pow1 = pow_uint(5, exp1);
    seq_t pow2 = pow_uint(5, exp2);
    seq_t pow3 = pow_uint(5, exp3);

    /* sum the terms: pow1 + pow3 + 2*pow2 */
    seq_t sum = pow1 + pow3 + pow2 + pow2;  /* adding pow2 twice */
    /* divide by 4 */
    return sum / 4;
}
