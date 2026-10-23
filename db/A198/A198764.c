/*
 * A198764  a(n) = 6*5^n-1.
 * https://oeis.org/A198764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*5^n-1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   mul_small, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    seq_t p   = seq_pow(5, n);   /* compute 5^n */
    seq_t res = mul_small(p, 6); /* multiply by 6 */
    res -= 1;                    /* subtract 1 */
    return res;
}
