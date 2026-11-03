/*
 * A139566  a(n) is the sum of squares of digits of a(n-1); a(1)=15.
 * https://oeis.org/A139566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 85 terms through n = 982 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=15; a(n)=sum of squares of digits of a(n-1) */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helper: sum of squares of decimal digits of a 64‑bit integer. */
static uint64_t digit_square_sum(uint64_t x)
{
    uint64_t sum = 0;
    while (x > 0) {
        uint64_t d = x % 10;
        sum += d * d;
        x /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Base case */
    if (n == (idx_t)1)
        return (seq_t)15;

    /* Iterate from a(2) up to a(n) */
    seq_t prev = (seq_t)15;
    for (idx_t i = (idx_t)2; i <= n; ++i)
    {
        /* digit_square_sum operates on uint64_t, the values here are < 200 */
        uint64_t sum = digit_square_sum((uint64_t)prev);
        prev = (seq_t)sum;
    }
    return prev;
}
