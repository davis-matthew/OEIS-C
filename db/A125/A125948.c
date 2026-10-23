/*
 * A125948  a(n) is the number of nonnegative integers k less than 10^n such
 *          that the decimal representation of k lacks at least one of digits
 *          1,2,3, at least one of digits 4,5,6 and at least one of digits
 *          7,8,9.
 * https://oeis.org/A125948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 27*7^n - 81*6^n + 108*5^n - 81*4^n + 36*3^n - 9*2^n + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* use the provided big_pow and seq_mul from utils */
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Compute each power */
    seq_t pow7 = big_pow((seq_t)7, (seq_t)n);
    seq_t pow6 = big_pow((seq_t)6, (seq_t)n);
    seq_t pow5 = big_pow((seq_t)5, (seq_t)n);
    seq_t pow4 = big_pow((seq_t)4, (seq_t)n);
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);

    /* Compute the weighted sum according to the formula */
    seq_t result = seq_mul((seq_t)27, pow7);
    result -= seq_mul((seq_t)81, pow6);
    result += seq_mul((seq_t)108, pow5);
    result -= seq_mul((seq_t)81, pow4);
    result += seq_mul((seq_t)36, pow3);
    result -= seq_mul((seq_t)9, pow2);
    result += (seq_t)1;

    return result;
}
