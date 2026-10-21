/*
 * A077503  Smallest n-digit square beginning with n.
 * https://oeis.org/A077503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = ceiling(sqrt(n*10^d))^2 where d = n - digit(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   digit_count, floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/floor_sqrt.h"
#include "../utils/digit_count.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 in the intended domain */
    idx_t d = n - digit_count(n);          /* digits beyond the leading pattern */
    seq_t mult = (seq_t)n;                 /* cast index to seq_t for multiplication */
    seq_t powv = pow10(d);                 /* 10^d as a seq_t value */
    seq_t m = mult * powv;                 /* n * 10^d */

    seq_t s = floor_sqrt(m);               /* floor(sqrt(m)) */
    /* If not a perfect square, increase to the next integer */
    if (s * s < m)
        s += 1;

    return s * s;                          /* square the adjusted root */
}
