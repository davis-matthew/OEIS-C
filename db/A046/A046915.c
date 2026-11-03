/*
 * A046915  Sum of divisors of 10^n.
 * https://oeis.org/A046915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1/4*(2^(n+1)-1)*(5^(n+1)-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 988 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

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
    seq_t two = pow2(n + 1);   /* 2^(n+1) */
    two -= 1;                  /* 2^(n+1) - 1 */

    seq_t five = power5(n + 1); /* 5^(n+1) */
    five -= 1;                  /* 5^(n+1) - 1 */

    seq_t prod = two * five;    /* (2^(n+1)-1)*(5^(n+1)-1) */

    return prod >> 2;           /* divide by 4 (shifts right 2) */
}
