/*
 * A228909  a(n) = 7^n - 6*6^n + 15*5^n - 20*4^n + 15*3^n - 6*2^n + 1.
 * https://oeis.org/A228909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n - 6*6^n + 15*5^n - 20*4^n + 15*3^n - 6*2^n + 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Compute the powers once */
    seq_t p7 = pow_seq((seq_t)7, (int)n);
    seq_t p6 = pow_seq((seq_t)6, (int)n);
    seq_t p5 = pow_seq((seq_t)5, (int)n);
    seq_t p4 = pow_seq((seq_t)4, (int)n);
    seq_t p3 = pow_seq((seq_t)3, (int)n);
    seq_t p2 = pow_seq((seq_t)2, (int)n);

    /* Evaluate the linear combination */
    seq_t result = p7;
    result -= (seq_t)6 * p6;
    result += (seq_t)15 * p5;
    result -= (seq_t)20 * p4;
    result += (seq_t)15 * p3;
    result -= (seq_t)6 * p2;
    result += (seq_t)1;

    return result;
}
