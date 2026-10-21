/*
 * A214698  a(n) = (n^n - n^2)/2.
 * https://oeis.org/A214698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^n - n^2)/2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* compute n^n */
    seq_t pow_n = pow_uint((uint64_t)n, n);

    /* compute n^2 */
    seq_t nn   = (seq_t)n;
    seq_t sq   = nn * nn;

    /* a(n) = (pow_n - sq) / 2 */
    seq_t res  = pow_n - sq;
    res = res >> 1;   /* divide by 2 (shift is safe for unsigned) */

    return res;
}
