/*
 * A047897  a(1) = 5; for n > 0, a(n+1) = a(n) * sum of digits of a(n).
 * https://oeis.org/A047897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(1) = 5; a(n+1) = a(n) * sum of digits of a(n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined for n < 1 */
    seq_t val = (seq_t)5;            /* a(1) */
    if (n == 1) return val;
    for (idx_t i = 1; i < n; ++i)
    {
        val = val * digit_sum(val);
    }
    return val;
}
