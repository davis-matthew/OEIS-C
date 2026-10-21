/*
 * A075106  Denominator of n/floor(log_2(n)); numerator is A075105(n).
 * https://oeis.org/A075106
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = floor(log2(n)) / gcd(n, floor(log2(n)))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0)
    {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* For n < 2 the sequence is undefined; return 1 (offset behaviour) */
    if (n < 2)
        return (seq_t)1;

    /* floor(log2(n)) – n is guaranteed to be >= 2 */
    uint64_t d = 63 - __builtin_clzll(n);

    /* Compute the greatest common divisor of n and d */
    uint64_t g = gcd_uint64(n, d);

    /* Denominator of the reduced fraction n/d */
    return (seq_t)(d / g);
}
