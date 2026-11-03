/*
 * A073752  Greatest common divisor of n/lpf(n) and n/gpf(n) where lpf(n) is
 *          the least and gpf(n) is the greatest prime factor of n (see
 *          A020639, A006530).
 * https://oeis.org/A073752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n/lpf(n), n/gpf(n))
 * Range:     u8 n <= 510, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 117 terms through n = 995 (2026-11-01)
 * Helpers:   gcd, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/largest_prime_factor.h"

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
    if (n <= 1)               /* a(1) = 1, a(0) not defined but harmless */
        return (seq_t)1;

    idx_t lpf_val = lpf(n);               /* least prime factor */
    uint64_t gpf_val = largest_prime_factor((uint64_t)n);  /* greatest prime factor */

    seq_t part1 = (seq_t)n / (seq_t)lpf_val;   /* n / lpf(n) */
    seq_t part2 = (seq_t)n / (seq_t)gpf_val;   /* n / gpf(n) */

    return gcd(part1, part2);       /* gcd of the two parts */
}
