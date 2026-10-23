/*
 * A117365  a(n) = largest prime less than the smallest prime dividing n (or 1
 *          if there is no such prime).
 * https://oeis.org/A117365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime less than the smallest prime dividing n (1
 *            if no such prime exists)
 * Range:     u8 n <= 262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Simple primality test for values up to 2^64‑1. */
static bool is_prime_u64(uint64_t v)
{
    if (v < 2) return false;
    if (v == 2) return true;
    if ((v & 1ULL) == 0) return false;
    for (uint64_t i = 3; i * i <= v; i += 2)
        if (v % i == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    /* n >= 1 by specification */
    if (n <= 1)                /* a(1) */
        return (seq_t)1;
    if ((n & 1ULL) == 0)       /* even numbers */
        return (seq_t)1;

    /* n is odd and >= 3: find the smallest prime factor */
    uint64_t un = (uint64_t)n;
    uint64_t spf = 0;
    for (uint64_t p = 3; p * p <= un; p += 2) {
        if (un % p == 0) {
            spf = p;
            break;
        }
    }
    if (spf == 0)               /* n itself is prime */
        spf = un;

    /* Find the largest prime less than spf */
    if (spf <= 2)                /* only possible for spf = 2 (n=2) */
        return (seq_t)1;
    uint64_t candidate = spf - 1;
    while (candidate >= 2) {
        if (is_prime_u64(candidate))
            break;
        candidate--;
    }
    if (candidate < 2)
        return (seq_t)1;
    return (seq_t)candidate;
}
