/*
 * A170740  Expansion of g.f.: (1+x)/(1-20*x).
 * https://oeis.org/A170740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 1 when n = 0;
 * a(n) = 21 * 20^(n-1) for n > 0
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    // a(1) = 21
    seq_t value = (seq_t)21;
    if (n == 1)
        return value;
    // For n >= 2 multiply by 20 (n-1) times
    for (idx_t i = 2; i <= n; ++i)
        value *= (seq_t)20;
    return value;
}
