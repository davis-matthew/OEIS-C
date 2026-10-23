/*
 * A181639  Numbers n such that omega(n) = digit-reverse(n).
 * https://oeis.org/A181639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 20 * 10^((n-1)/2) for odd n, a(n) = 30 * 10^(n/2 - 1) for
 *            even n
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* offset is 1 – a(1) is the first term. */
    if (n == 0) return 0;          /* undefined but safe */
    bool odd = (n & 1u);
    idx_t exp;
    if (odd) {
        exp = (n - 1) / 2;         /* exponent for 20·10^exp */
    } else {
        exp = n / 2 - 1;           /* exponent for 30·10^exp */
    }
    seq_t powten = pow10(exp);     /* 10^exp as seq_t */
    return odd ? (powten * 20) : (powten * 30);
}
