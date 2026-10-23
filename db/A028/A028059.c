/*
 * A028059  Expansion of 1/((1-3x)(1-5x)(1-7x)(1-8x)).
 * https://oeis.org/A028059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (8^(n+4)-15*7^(n+3)+2*5^(n+4)-3^(n+4))/120
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Convert exponents to seq_t */
    seq_t exp8 = (seq_t)(n + 4);
    seq_t exp7 = (seq_t)(n + 3);
    seq_t exp5 = exp8;                 /* same as n+4 */
    seq_t exp3 = exp8;                 /* same as n+4 */

    /* Compute powers */
    seq_t p8  = big_pow((seq_t)8,  exp8);
    seq_t p7  = big_pow((seq_t)7,  exp7);
    seq_t p5  = big_pow((seq_t)5,  exp5);
    seq_t p3  = big_pow((seq_t)3,  exp3);

    /* Combine terms with exact integer arithmetic */
    seq_t term = p8;
    term -= mul_small(p7, 15);
    term += mul_small(p5, 2);
    term -= p3;

    /* Exact division by 120 */
    return term / (seq_t)120;
}
