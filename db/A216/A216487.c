/*
 * A216487  Smallest prime factor of n^(2n) - 1 having the form k*n+1.
 * https://oeis.org/A216487
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 18, u32 n <= 18, u64 n <= 670,
 *            u128 n <= 670, big any n
 * Helpers:   is_prime_big, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the smallest prime divisor of n^(2n) - 1 that is congruent to 1 (mod n). */
/* a(n) = Min{p | p prime, p ≡ 1 (mod n), p | n^(2n) - 1}  (for n ≥ 2) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 2u) return 0;
    uint64_t nn = (uint64_t)n;

    for (uint64_t k = 1;; ++k) {
        uint64_t p = nn * k + 1;
        /* reject if not prime */
        if (!is_prime_big((seq_t)p)) continue;
        /* test if n^(2n) ≡ 1 (mod p) */
        seq_t res = mod_pow((seq_t)nn, 2 * nn, (seq_t)p);
        if (res == 1) return (seq_t)p;
    }
}
