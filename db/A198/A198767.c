/*
 * A198767  a(n) = 8*5^n-1.
 * https://oeis.org/A198767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*5^n-1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 970 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 5*a(n-1)+4, a(0) = 7 */

#include <stdbool.h>
#include <stdint.h>

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
    seq_t val = power5(n);           /* val = 5^n                     */
    val = val * (seq_t)8;            /* val = 8*5^n                   */
    val = val - (seq_t)1;            /* val = 8*5^n - 1               */
    return val;
}
