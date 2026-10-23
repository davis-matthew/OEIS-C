/*
 * A024106  a(n) = 9^n-n^5.
 * https://oeis.org/A024106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n - n^5
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   pow_uint, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t pow9 = pow_uint(9ULL, n);          /* 9^n */
    seq_t pn   = (seq_t)n;                   /* n as seq_t */
    seq_t pow5 = seq_pow(pn, 5);             /* n^5 */
    return pow9 - pow5;
}
