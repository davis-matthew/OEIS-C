/*
 * A032122  Number of reversible strings with n beads of 5 colors.
 * https://oeis.org/A032122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5^n + 5^floor((n+1)/2)) / 2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 485 (2026-11-01)
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
    idx_t half = (n + 1) >> 1;
    seq_t pow_n = power5(n);
    seq_t pow_half = power5(half);
    seq_t sum = pow_n + pow_half;
    return sum / (seq_t)2;
}
