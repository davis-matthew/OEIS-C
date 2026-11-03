/*
 * A125831  a(n) = (5^n - 1)/2.
 * https://oeis.org/A125831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (5^n - 1)/2 Equivalent recurrence: a(0)=0,
 *            a(n)=5*a(n-1)+2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 296 (2026-11-01)
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
    seq_t r = power5(n);  /* r = 5^n */
    r -= 1;               /* r = 5^n - 1 */
    r /= 2;               /* r = (5^n - 1)/2 */
    return r;
}
