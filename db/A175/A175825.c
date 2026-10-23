/*
 * A175825  Maximum signed integer that can be stored in n bytes.
 * https://oeis.org/A175825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, less, nonn
 * Formula:   a(n) = 2^(8*n-1) - 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* n is guaranteed to be at least 1 for the OEIS offset */
    idx_t exp = 8ULL * n - 1ULL;           /* exponent 8*n-1 */
    seq_t v   = pow_uint(2ULL, exp);       /* compute 2^(8*n-1) */
    return v - (seq_t)1;                   /* subtract 1 */
}
