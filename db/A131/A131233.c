/*
 * A131233  a(n) is the number of positive integers <= n that do not have 2 or
 *          more distinct prime divisors in common with n.
 * https://oeis.org/A131233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers m ≤ n such that gcd(m,n) is 1 or a
 *            prime power
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 8718 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Works for x up to 64‑bit values; the routine is fast for small x. */
static inline bool is_prime_power_u64(uint64_t x)
{
    if (x <= 1) return true;          /* 0 and 1 are considered prime powers (p^0) */

    uint64_t p = 0;

    /* Find the smallest prime factor of x. */
    if ((x & 1) == 0) {           /* 2 is a factor */
        p = 2;
    } else {
        for (uint64_t d = 3; d <= x / d; d += 2) {
            if (x % d == 0) {
                p = d;
                break;
            }
        }
    }

    /* If no factor found, x is prime. */
    if (p == 0) return true;

    /* Remove all occurrences of the found prime factor. */
    while (x % p == 0) x /= p;

    /* Remaining x is 1 => x is a power of p; otherwise it contains another prime factor. */
    return x == 1;
}

/* Test whether x is a prime power (x ≥ 0).  Returns true for 0,1 or any prime power. */

seq_t a(idx_t n)
{
    seq_t count = 0;

    for (idx_t m = 1; m <= n; ++m) {
        seq_t g = gcd((seq_t)m, (seq_t)n);
        if (is_prime_power_u64((uint64_t)g))
            count++;
    }

    return count;
}
