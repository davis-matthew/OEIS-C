/*
 * A122041  a(n) = 2*a(n-1) - 1 for n>1, a(1)=23.
 * https://oeis.org/A122041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*2^n + 1 (for n >= 1)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n < 1 is undefined; return 0 for safety */
    if (n < 1)
        return (seq_t)0;

    seq_t pow2n = pow2(n);          /* 2^n in seq_t */
    seq_t result = pow2n * (seq_t)11;
    result += (seq_t)1;
    return result;
}
