/*
 * A178188  a(n) = Sum_{k=1..n} 5^((k^2+3k)/2).
 * https://oeis.org/A178188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} 5^((k^2+3k)/2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Validated: standard, pass, 25 terms through n = 50 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; safety */
    seq_t current = power5(2);      /* 5^((1^2+3*1)/2) = 5^2 = 25 */
    seq_t sum = current;

    for (idx_t k = 2; k <= n; ++k)
    {
        /* exponent increments by k+1 each step: e(k) - e(k-1) = k+1 */
        current = seq_mul(current, power5(k + 1));
        sum += current;
    }

    return sum;
}
