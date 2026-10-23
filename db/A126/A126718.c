/*
 * A126718  a(n) is the number of nonnegative integers k less than 10^n such
 *          that the decimal representation of k lacks the digits 1,2,3, at
 *          least one of digits 4,5, at least one of digits 6,7 and at least
 *          one of digits 8,9.
 * https://oeis.org/A126718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = 8*4^n - 12*3^n + 6*2^n - 1 (offset 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the multinomial counting formula directly. */

#include <stdint.h>

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* n is guaranteed to be ≥ 1 by the contract. */
    seq_t pow4 = pow_seq((seq_t)4, n);
    seq_t pow3 = pow_seq((seq_t)3, n);
    seq_t pow2 = pow_seq((seq_t)2, n);

    seq_t term;
    term = (seq_t)8 * pow4;
    term -= (seq_t)12 * pow3;
    term += (seq_t)6  * pow2;
    term -= (seq_t)1;
    return term;
}
