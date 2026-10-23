/*
 * A036227  a(1) = 20; a(n+1) = a(n) + sum of decimal digits of a(n).
 * https://oeis.org/A036227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(1) = 20; a(n+1) = a(n) + digit_sum(a(n)) for n >= 1
 * Range:     u8 n <= 26, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* For n < 1 the behaviour is irrelevant, return 0 */
    if (n < 1)
        return 0;

    seq_t val = (seq_t)20;           /* a(1) */
    if (n == 1)                     /* if only the first term is requested */
        return val;

    /* Iterate from 2 to n, accumulating the sequence */
    for (idx_t i = 2; i <= n; ++i)
        val += digit_sum(val);

    return val;
}
