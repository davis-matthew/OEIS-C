/*
 * A013736  a(n) = 5^(3*n + 1).
 * https://oeis.org/A013736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^(3*n + 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 8, u128 n <= 18,
 *            big any n
 * Validated: standard, pass, 29 terms through n = 99 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline seq_t power5(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 5;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t exp = 3*n + 1;
    return power5(exp);
}
