/*
 * A036213  Duplicating binary multipliers; i.e., n+1 1-bits placed 2n bits
 *          from each other.
 * https://oeis.org/A036213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (2^(2*n^2+2*n)-1)/(2^(2*n)-1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"   /* pow2(idx_t exp) -> seq_t 2^exp */

seq_t a(idx_t n)
{
    if (n == 0) return 1;                 /* a(0) = 1 */

    /* Compute the exponents for the numerator and denominator */
    idx_t exp_num = (idx_t)2 * n * n + (idx_t)2 * n;  /* 2*n^2 + 2*n */
    idx_t exp_den = (idx_t)2 * n;                     /* 2*n */

    /* 2^exp - 1 for numerator and denominator */
    seq_t numerator   = pow2(exp_num) - 1;   /* 2^(exp_num) - 1 */
    seq_t denominator = pow2(exp_den) - 1;   /* 2^(exp_den) - 1 */

    return numerator / denominator;          /* exact division */
}
