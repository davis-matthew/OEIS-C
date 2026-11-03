/*
 * A198402  a(n) = 5^n * n^5.
 * https://oeis.org/A198402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n * n^5
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 18, u128 n <= 43,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 926 (2026-11-01)
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
    if (n == 0)
        return 0;
    seq_t pow5n = power5(n);          /* 5^n */
    seq_t base   = (seq_t)n;          /* n */
    seq_t n5 = base;
    n5 *= base; /* n^2 */
    n5 *= base; /* n^3 */
    n5 *= base; /* n^4 */
    n5 *= base; /* n^5 */
    return pow5n * n5;
}
