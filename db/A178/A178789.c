/*
 * A178789  a(n) = 4^(n-1) + 2: Number of acute angles after n iterations of
 *          the Koch snowflake construction.
 * https://oeis.org/A178789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 + 4^(n-1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For safety we return 0 when n == 0. */
    if (n == 0) return 0;
    seq_t val = pow4(n - 1);   /* compute 4^(n-1) */
    val += 2;                  /* add the constant term */
    return val;
}
