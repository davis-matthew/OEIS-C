/*
 * A028054  Expansion of 1/((1-3x)(1-5x)(1-6x)(1-8x)).
 * https://oeis.org/A028054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (8^(n+3) - 5*6^(n+3) + 5^(n+4) - 3^(n+3)) / 30
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Exponents for the required powers */
    seq_t exp_n3 = (seq_t)(n + 3);
    seq_t exp_n4 = (seq_t)(n + 4);

    /* Compute the powers using the provided big_pow helper */
    seq_t pow8  = big_pow((seq_t)8, exp_n3);
    seq_t pow6  = big_pow((seq_t)6, exp_n3);
    seq_t pow3  = big_pow((seq_t)3, exp_n3);
    seq_t pow5n = big_pow((seq_t)5, exp_n4);  /* 5^(n+4) */

    /* Evaluate the numerator of the formula */
    seq_t term5pow6 = pow6 * (seq_t)5;               /* 5 * 6^(n+3) */
    seq_t numerator = pow8 - term5pow6 + pow5n - pow3; /* 8^(n+3) - 5*6^(n+3) + 5^(n+4) - 3^(n+3) */

    /* Divide by 30 to obtain the final result */
    seq_t result = numerator / (seq_t)30;

    return result;
}
