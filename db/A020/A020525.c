/*
 * A020525  a(n) = 5th Euler polynomial evaluated at 2^n and multiplied by 2.
 * https://oeis.org/A020525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(5*n+1) - 5*2^(4*n) + 5*2^(2*n) - 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    seq_t two = (seq_t)2;

    /* compute exponents as seq_t to avoid overflow of idx_t */
    seq_t exp5n1 = (seq_t)(5 * n + 1);
    seq_t exp4n   = (seq_t)(4 * n);
    seq_t exp2n   = (seq_t)(2 * n);

    /* powers of two */
    seq_t pow5n1 = big_pow(two, exp5n1);
    seq_t pow4n  = big_pow(two, exp4n);
    seq_t pow2n  = big_pow(two, exp2n);

    /* combine according to the formula */
    seq_t res = pow5n1;
    res -= (seq_t)5 * pow4n;
    res += (seq_t)5 * pow2n;
    res -= (seq_t)1;

    return res;
}
