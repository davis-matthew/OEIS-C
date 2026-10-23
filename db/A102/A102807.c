/*
 * A102807  a(n) is the square of one plus the number consisting of n 3's.
 * https://oeis.org/A102807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = ((10^n + 2)/3)^2 = (100^n + 4*10^n + 4)/9
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t t = pow10(n);   /* 10^n */
    t += 2;               /* 10^n + 2 */
    t /= 3;               /* (10^n + 2)/3  (division is exact) */
    return t * t;         /* square the result */
}
