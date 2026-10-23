/*
 * A126634  a(n) is the number of nonnegative integers k less than 10^n such
 *          that the decimal representation of k lacks the digits 1,2,3,4 and
 *          at least one of digits 5,6,7,8,9.
 * https://oeis.org/A126634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5*5^n - 10*4^n + 10*3^n - 5*2^n + 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: compute base^exp using repeated multiplication */
static seq_t power_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    /* n >= 1 for the defined offset */
    seq_t pow5 = power_seq((seq_t)5, n);
    seq_t pow4 = power_seq((seq_t)4, n);
    seq_t pow3 = power_seq((seq_t)3, n);
    seq_t pow2 = power_seq((seq_t)2, n);

    seq_t res = 0;
    res += (seq_t)5 * pow5;
    res -= (seq_t)10 * pow4;
    res += (seq_t)10 * pow3;
    res -= (seq_t)5 * pow2;
    res += (seq_t)1;

    return res;
}
