/*
 * A086966  Number of distinct zeros of x^4-x-1 mod prime(n).
 * https://oeis.org/A086966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct zeros of x^4 - x - 1 modulo the n-th
 *            prime
 * Range:     u8 n <= 54, u16 n <= 7057, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We compute the n-th prime by trial division using the provided
   is_prime function, then test every field element x ∈ [0, p-1]
   for the polynomial value. The number of zeros is at most 4,
   so the simple enumeration is fast enough for primes up to ~10⁵. */

#include <stdint.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t) */
#include "../utils/mod_mul.h"   /* uint64_t mod_mul(uint64_t, uint64_t, uint64_t) */

seq_t a(idx_t n)
{
    /* Find the n-th prime. */
    idx_t count = 0;
    uint64_t candidate = 2;
    uint64_t prime64 = 0;

    while (1) {
        if (is_prime(candidate)) {
            ++count;
            if (count == n) {
                prime64 = candidate;
                break;
            }
        }
        /* After 2, skip even numbers. */
        candidate += (candidate == 2 ? 1 : 2);
    }

    seq_t p = (seq_t)prime64;
    seq_t zeros = 0;

    for (seq_t x = 0; x < p; ++x) {
        /* x^2 mod p */
        uint64_t x64 = (uint64_t)x;
        uint64_t x2 = mod_mul(x64, x64, prime64);
        /* x^4 mod p */
        uint64_t x4 = mod_mul(x2, x2, prime64);
        /* polynomial value: x^4 - x - 1 (mod p) */
        uint64_t poly = (x4 + prime64 - (x64 + 1)) % prime64;
        if (poly == 0)
            ++zeros;
    }

    return zeros;
}
