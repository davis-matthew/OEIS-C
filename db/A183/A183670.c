/*
 * A183670  Number of (n+1) X 8 0..6 arrays with every 2 X 2 subblock summing
 *          to 12.
 * https://oeis.org/A183670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*7^n + 1524*6^n + 30250*5^n + 210680*4^n + 798342*3^n +
 *            1927804*2^n + 2796194
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 20, u128 n <= 44,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* compute the powers once */
    seq_t pow7 = seq_pow((seq_t)7, n);
    seq_t pow6 = seq_pow((seq_t)6, n);
    seq_t pow5 = seq_pow((seq_t)5, n);
    seq_t pow4 = seq_pow((seq_t)4, n);
    seq_t pow3 = seq_pow((seq_t)3, n);
    seq_t pow2 = seq_pow((seq_t)2, n);

    /* combine the terms */
    seq_t result = 0;
    result += (seq_t)7       * pow7;
    result += (seq_t)1524    * pow6;
    result += (seq_t)30250   * pow5;
    result += (seq_t)210680  * pow4;
    result += (seq_t)798342  * pow3;
    result += (seq_t)1927804 * pow2;
    result += (seq_t)2796194;

    return result;
}
