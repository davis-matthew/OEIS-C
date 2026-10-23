/*
 * A170638  Number of reduced words of length n in Coxeter group on 5
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = 5 * 4^(n-1) for n >= 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"   /* provides seq_t pow_uint(uint64_t base, idx_t exp) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    /* compute 4^(n-1) */
    seq_t pow4 = pow_uint(4, n - 1);
    /* multiply by 5 */
    return pow4 * (seq_t)5;
}
