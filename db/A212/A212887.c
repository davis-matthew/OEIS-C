/*
 * A212887  a(n) is the prime p corresponding to the smallest integer k such
 *          that k^2 == p (mod prime(n)).
 * https://oeis.org/A212887
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the prime p such that p ≡ k^2 (mod prime(n)) for the
 *            smallest k ≥ 1; return that prime
 * Range:     u8 n <= 54, u16 n <= 6693, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 4)
        return 0; /* values below the offset are undefined */

    /* get the n‑th prime (1‑indexed) */
    seq_t p = (seq_t)get_prime(n);

    /* search for the smallest k with k^2 mod p prime */
    uint64_t prime_val = (uint64_t)p; /* p fits in 64 bits for the problem bounds */
    for (uint64_t k = 1;; ++k) {
        uint64_t r = (k * k) % prime_val;
        if (is_prime_big((seq_t)r))
            return (seq_t)r;
    }
}
