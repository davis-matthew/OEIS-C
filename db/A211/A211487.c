/*
 * A211487  Characteristic sequence of numbers n having a primitive root
 *          modulo n.
 * https://oeis.org/A211487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 iff n = 2, 4, p^k, or 2*p^k (p odd prime)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9594 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint8_t is_odd_prime_power(uint64_t m)
{
    if (m < 3) return 0;                      /* must be >=3 for odd prime powers */
    uint64_t p = 0;
    /* trial division to find the smallest odd divisor */
    for (uint64_t d = 3; d <= m / d; d += 2) {
        if (m % d == 0) {                     /* found a divisor d */
            p = d;
            break;
        }
    }
    if (p == 0) return 1;                     /* m itself is an odd prime */
    /* verify that all prime factors are the same */
    while (m % p == 0)
        m /= p;
    return m == 1;                            /* true if m reduced to 1 */
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 4) return (seq_t)1;

    if ((n & 1ULL) == 0) {                     /* even n */
        uint64_t m = n >> 1;                   /* m = n / 2 */
        if ((m & 1ULL) == 0)                   /* m even → not allowed */
            return (seq_t)0;
        return (seq_t)is_odd_prime_power(m);
    } else {                                   /* odd n */
        return (seq_t)is_odd_prime_power(n);
    }
}
