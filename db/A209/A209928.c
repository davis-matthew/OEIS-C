/*
 * A209928  Largest digit of all divisors of n.
 * https://oeis.org/A209928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = maximum decimal digit appearing in any divisor of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9810 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint8_t largest_digit_u64(idx_t v)
{
    uint8_t max = 0;
    while (v > 0)
    {
        uint8_t d = v % 10;
        if (d > max) max = d;
        v /= 10;
    }
    return max;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* safeguard, although n >= 1 by definition */
    uint8_t maxDigit = 0;
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            uint8_t d1 = largest_digit_u64(i);
            if (d1 > maxDigit)
            {
                maxDigit = d1;
                if (maxDigit == 9) return (seq_t)9;
            }
            idx_t j = n / i;
            if (i != j)
            {
                uint8_t d2 = largest_digit_u64(j);
                if (d2 > maxDigit)
                {
                    maxDigit = d2;
                    if (maxDigit == 9) return (seq_t)9;
                }
            }
        }
    }
    return (seq_t)maxDigit;
}
