/*
 * A127065  a(n) = n! - (n-2)^2.
 * https://oeis.org/A127065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! - (n-2)^2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    long long diff = (long long)n - 2;          /* n-2 in signed form   */
    seq_t square = (seq_t)(diff * diff);        /* (n-2)^2              */

    return fact - square;
}
