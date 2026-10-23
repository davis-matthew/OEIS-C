/*
 * A028234  If n = p_1^e_1 * ... * p_k^e_k, p_1 < ... < p_k primes, then a(n)
 *          = n/p_1^e_1, with a(1) = 1.
 * https://oeis.org/A028234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = n divided by the largest power of its least prime factor
 *            (the largest divisor of n not divisible by its smallest prime
 *            factor)
 * Range:     u8 n <= 513, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Smallest prime factor of n (n >= 2). */
static uint64_t _lpf(uint64_t n)
{
    if ((n & 1ULL) == 0ULL) return 2ULL;
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL)
        if ((n % d) == 0ULL) return d;
    return n;
}

seq_t a(idx_t n)
{
    /* a(1) = 1 */
    if (n == 1ULL) return 1;

    uint64_t x = (uint64_t)n;
    uint64_t p = _lpf(x);          /* least prime factor */
    while ((x % p) == 0ULL)        /* remove that prime power */
        x /= p;
    return (seq_t)x;
}
