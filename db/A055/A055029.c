/*
 * A055029  Number of inequivalent Gaussian primes of norm n.
 * https://oeis.org/A055029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 1 if n==2; 2 if n is prime ≡1 (mod4); 1 if n = p² where p
 *            is prime ≡3 (mod4); 0 otherwise
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* Compute ⌊sqrt(x)⌋ for a 64‑bit unsigned integer.  */
static inline idx_t uint64_isqrt(idx_t x)
{
    idx_t root = 0;
    idx_t bit = 1ULL << 62;                       /* largest power of 4 <= 2^64-1 */

    /* shift bit down until it is <= x */
    while (bit > x)
        bit >>= 2;

    while (bit != 0) {
        if (x >= root + bit) {
            x -= root + bit;
            root = (root >> 1) + bit;
        } else {
            root >>= 1;
        }
        bit >>= 2;
    }
    return root;
}

seq_t a(idx_t n)
{
    if (n < 2u) return (seq_t)0;

    if (n == 2u) return (seq_t)1;

    if (is_prime(n)) {
        /* n is a rational prime */
        return ((n & 3u) == 1u) ? (seq_t)2 : (seq_t)0;
    }

    /* Check if n is a square of a prime ≡3 (mod4) */
    idx_t r = uint64_isqrt(n);
    if (r * r == n && is_prime(r) && ((r & 3u) == 3u))
        return (seq_t)1;

    return (seq_t)0;
}
