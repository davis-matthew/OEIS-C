/*
 * A162962  a(n) = 5*a(n-2) for n > 2; a(1) = 1, a(2) = 5.
 * https://oeis.org/A162962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^(⌊n/2⌋) Recurrence: a(n) = 5*a(n-2) for n > 2; a(1)=1,
 *            a(2)=5
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-01)
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

seq_t a(idx_t n) {
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1 */
    idx_t e = n >> 1;              /* floor(n/2) */
    return power5(e);
}
