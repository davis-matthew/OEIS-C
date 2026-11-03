/*
 * A020639  Lpf(n): least prime dividing n (when n > 1); a(1) = 1. Or,
 *          smallest prime factor of n, or smallest prime divisor of n.
 * https://oeis.org/A020639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, core
 * Formula:   a(n) = least prime factor of n, with a(1) = 1
 * Range:     u8 n <= 256, u16 n <= 65536, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 113 terms through n = 9365 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return the smallest prime factor of n   (n > 1) */
static inline idx_t lpf(idx_t n)
{
    if (n % 2 == 0) return 2;
    for (idx_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return i;
    return n;   /* n itself is prime – should not be called for primes */
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    return (seq_t)lpf(n);
}
