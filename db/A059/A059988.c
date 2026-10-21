/*
 * A059988  a(n) = (10^n - 1)^2.
 * https://oeis.org/A059988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (10^n - 1)^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute 10^n as a seq_t, then subtract 1 and square the result. */
    seq_t t = pow10(n);   /* t = 10^n  */
    t -= 1;               /* t = 10^n - 1  */
    return t * t;         /* a(n) = (10^n - 1)^2  */
}
