/*
 * A134008  a(n) = 1^n + 3^n + 5^n + 7^n + 9^n + 11^n.
 * https://oeis.org/A134008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1^n + 3^n + 5^n + 7^n + 9^n + 11^n
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* 1^n is 1 for any n >= 0, so add it directly. */
    seq_t sum = 1;

    /* Compute each odd base raised to the power n and accumulate. */
    sum += seq_pow((seq_t)3,  n);
    sum += seq_pow((seq_t)5,  n);
    sum += seq_pow((seq_t)7,  n);
    sum += seq_pow((seq_t)9,  n);
    sum += seq_pow((seq_t)11, n);

    return sum;
}
