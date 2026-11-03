/*
 * A128784  a(n) = n^2*5^n.
 * https://oeis.org/A128784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*5^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 50,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 997 (2026-11-01)
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
    seq_t pow5 = power5(n);    /* compute 5^n */
    seq_t nn = (seq_t)n;
    nn = nn * nn;              /* n^2 */
    return nn * pow5;          /* n^2 * 5^n */
}
