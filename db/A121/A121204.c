/*
 * A121204  a(n) = -2*n + 7^n - 5^n.
 * https://oeis.org/A121204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -2*n + 7^n - 5^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* compute 7^n and 5^n */
    seq_t pow7 = big_pow((seq_t)7, (seq_t)n);
    seq_t pow5 = big_pow((seq_t)5, (seq_t)n);

    /* a(n) = (7^n - 5^n) - 2*n */
    seq_t result = pow7 - pow5;
    seq_t two_n  = (seq_t)n * (seq_t)2;
    result -= two_n;

    return result;
}
