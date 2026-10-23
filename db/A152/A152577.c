/*
 * A152577  a(n) = 10^(2*n - 1) + 1.
 * https://oeis.org/A152577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^(2*n - 1) + 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* n must be >= 1 (OEIS offset is 1) */
    idx_t exp = n;    /* start with n */
    exp += n;         /* exp = 2*n */
    exp -= 1;         /* exp = 2*n - 1 */
    seq_t value = pow10(exp);   /* compute 10^(2*n-1) */
    value += 1;                  /* add the trailing 1 */
    return value;
}
