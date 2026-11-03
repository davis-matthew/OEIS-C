/*
 * A177980  Iterate (n + lpf(n)) / 2 until a prime is reached, where lpf
 *          equals the least prime factor. a(n) is that terminating prime.
 * https://oeis.org/A177980
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 30000, u32 n <= 30000, u64 n <= 30000,
 *            u128 n <= 30000, big any n
 * Validated: standard, pass, 89 terms through n = 9967 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Description: for each n ≥ 2 compute a(n) by iterating (m + lpf(m))/2 until m is prime, then return that prime. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

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
    /* For n < 2 the OEIS offset is undefined; return 0 in that case. */
    if (n < 2)
        return (seq_t)0;

    idx_t m = n;
    while (!is_prime(m)) {
        idx_t l = lpf(m);          /* least prime factor of m (returns m if m is prime) */
        m = (m + l) / 2;           /* integer division; m never exceeds its previous value */
    }
    return (seq_t)m;               /* terminating prime */
}
