/*
 * A198763  a(n) = 4*5^n-1.
 * https://oeis.org/A198763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*5^n-1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t p      = seq_pow(5, n);    /* compute 5^n        */
    seq_t result = seq_mul(p, 4);    /* multiply by 4      */
    result = result - 1;             /* subtract 1         */
    return result;
}
