/*
 * A182936  Greatest common divisor of the proper divisors of n, 0 if there
 *          are none.
 * https://oeis.org/A182936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is not composite, a(n) = p if n is a proper power
 *            of prime p, a(n) = 1 otherwise
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Static helper to test primality for 64‑bit integers.
   Only accepted by the framework if the build supplies it,
   otherwise use a simple trial division. */
static bool _is_prime(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

/* Return the greatest common divisor of the proper divisors of n. */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    uint64_t m = (uint64_t)n;

    /* quick prime test */
    if (_is_prime(m)) return 0;

    /* 1. Find the smallest prime factor of m. */
    uint64_t spf = 0;
    if (m % 2 == 0) {
        spf = 2;
    } else {
        for (uint64_t d = 3; d * d <= m; d += 2) {
            if (m % d == 0) {
                spf = d;
                break;
            }
        }
        if (spf == 0) spf = m;            /* m is prime (should not happen) */
    }

    /* 2. Check whether m is a pure power of spf. */
    uint64_t temp = m;
    while (temp % spf == 0) {
        temp /= spf;
    }

    if (temp == 1) {
        /* m = spf^k with k >= 2 */
        return (seq_t)spf;
    } else {
        /* not a pure prime power */
        return (seq_t)1;
    }
}
