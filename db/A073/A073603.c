/*
 * A073603  Smallest multiple of n-th prime which is == 1 mod (n+1)-st prime.
 * https://oeis.org/A073603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = prime(n) * inv(prime(n) mod prime(n+1)) where inv(x mod
 *            m) is the smallest k with (k*x)%m == 1
 * Range:     u8 n <= 7, u16 n <= 61, u32 n <= 6683, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use get_prime(idx_t k) from "../utils/get_prime.h" to fetch the
   k-th prime (k >= 1).  The modular inverse is computed with the
   extended Euclidean algorithm, implemented directly below. */

/* The function works for all n >= 1 and is independent of the order of
   calls.  No memoisation is needed because each evaluation is O(1). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

static uint64_t mod_inverse(uint64_t a, uint64_t m) {
    uint64_t b = m;
    int64_t x0 = 1, x1 = 0;
    while (b != 0) {
        uint64_t q = a / b;
        uint64_t r = a % b;
        a = b; b = r;

        int64_t t = x0 - (int64_t)q * x1;
        x0 = x1; x1 = t;
    }
    if (a != 1) return 0;           /* not invertible – should not happen */
    if (x0 < 0) x0 += (int64_t)m;   /* make it positive modulo m */
    return (uint64_t)x0;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n < 1 */

    /* Fetch successive primes */
    uint32_t p_n   = get_prime(n);
    uint32_t p_np1 = get_prime(n + 1);

    /* The modular inverse of p_n modulo p_np1 */
    uint64_t inv = mod_inverse((uint64_t)p_n, (uint64_t)p_np1);

    return (seq_t)p_n * (seq_t)inv;
}
