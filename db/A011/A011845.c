/*
 * A011845  a(n) = floor( binomial(n,8)/9).
 * https://oeis.org/A011845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( binomial(n,8) / 9 )
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* For n < 8, binomial(n,8) = 0, so a(n) = 0 */
    if (n < 8) return 0;

    /* Compute binomial(n,8) using multiplicative formula
       with exact division at each step to avoid overflow. */
    seq_t result = 1;
    /* k = 8 */
    for (idx_t i = 1; i <= 8; ++i)
    {
        seq_t num = (seq_t)(n - 8 + i);          /* n-k+i = n-8+i */
        seq_t g = gcd(num, (seq_t)i);           /* cancel common factors */
        num /= g;
        seq_t div = (seq_t)i / g;               /* divisor that will be cancelled with result */
        result /= div;                          /* exact division */
        result *= num;                          /* accumulate numerator */
    }

    /* Finally divide by 9 to obtain floor(binomial(n,8)/9) */
    result /= 9;
    return result;
}
