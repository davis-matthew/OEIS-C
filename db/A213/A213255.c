/*
 * A213255  2^(n-1) - floor((2^(n-1) - 1)/(n-1)).
 * https://oeis.org/A213255
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n-1) - floor((2^(n-1)-1)/(n-1))
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;           /* offset is 2, below that behaviour is ignored */
    idx_t exp = n - 1;              /* exponent for 2^(n-1) */
    seq_t pow_val = pow2(exp);      /* compute 2^(n-1) */
    seq_t numerator = pow_val - 1;  /* 2^(n-1) - 1 */
    seq_t denom = (seq_t)exp;       /* n-1 as seq_t */
    seq_t div = numerator / denom;  /* floor division */
    return pow_val - div;           /* a(n) */
}
