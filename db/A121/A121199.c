/*
 * A121199  a(n) = 12n+7^n+5^n.
 * https://oeis.org/A121199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*n + 7^n + 5^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Convert index to seq_t for arithmetic */
    seq_t tn = (seq_t)n;
    /* Compute 5^n and 7^n */
    seq_t pow5 = big_pow((seq_t)5, n);
    seq_t pow7 = big_pow((seq_t)7, n);
    /* 12*n term */
    seq_t term = tn * (seq_t)12;
    /* Sum all parts */
    return pow5 + pow7 + term;
}
