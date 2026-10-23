/*
 * A117857  Number of palindromes of length n (in base 5).
 * https://oeis.org/A117857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 4*5^floor((n-1)/2)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 26, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   mul_small, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_pow.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t k = (n - 1) / 2;                 /* exponent 5^k   */
    seq_t p = seq_pow((seq_t)5, k);         /* compute 5^k   */
    seq_t res = mul_small(p, 4);            /* multiply by 4  */
    return res;
}
