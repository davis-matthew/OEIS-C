/*
 * A026383  a(n) = 5*a(n-2), starting 1,2.
 * https://oeis.org/A026383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(1)=2; a(n)=5*a(n-2) for n ≥ 2
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 29 (2026-11-01)
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
    seq_t p = power5(n / 2);
    if ((n & 1) == 0)
        return p;
    return p * (seq_t)2;
}
