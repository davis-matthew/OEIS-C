/*
 * A019490  Expansion of 1/((1-4*x)*(1-6*x)*(1-12*x)).
 * https://oeis.org/A019490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n -3*6^n +3*12^n
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t pow4  = big_pow((seq_t)4,  (seq_t)n);
    seq_t pow6  = big_pow((seq_t)6,  (seq_t)n);
    seq_t pow12 = big_pow((seq_t)12, (seq_t)n);

    seq_t term12 = pow12 * (seq_t)3;   // 3 * 12^n
    seq_t term6  = pow6  * (seq_t)3;   // 3 * 6^n

    /* term12 >= term6, so subtraction is safe for unsigned types */
    return term12 + pow4 - term6;     // 3*12^n + 4^n - 3*6^n
}
