/*
 * A178351  Number of collinear point 5-tuples in a 7 X 7 X 7 X ...
 *          n-dimensional cubic grid.
 * https://oeis.org/A178351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*11^n + 13^n/2 + 5*9^n - 15*7^n/2
 * (all divisions are integer divisions, truncating toward 0)
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* compute powers */
    seq_t p11 = big_pow((seq_t)11, (seq_t)n);
    seq_t p13 = big_pow((seq_t)13, (seq_t)n);
    seq_t p9  = big_pow((seq_t)9,  (seq_t)n);
    seq_t p7  = big_pow((seq_t)7,  (seq_t)n);

    /* terms of the formula */
    seq_t term1 = (seq_t)2 * p11;                /* 2*11^n */
    seq_t term2 = p13 / (seq_t)2;                /* 13^n/2 */
    seq_t term3 = (seq_t)5 * p9;                 /* 5*9^n */
    seq_t term4 = (seq_t)15 * p7 / (seq_t)2;     /* (15*7^n)/2 */

    /* combine */
    seq_t result = term1 + term2 + term3;
    result -= term4;
    return result;
}
