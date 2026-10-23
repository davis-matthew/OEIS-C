/*
 * A042969  Primes p such that x^7 = 3 has no solution mod p.
 * https://oeis.org/A042969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 931, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p ≡ 1 (mod 7) such that 3 is not a 7th‑power residue mod p.
   Search by iterating primes and testing powmod(3, (p‑1)/7, p) != 1. */

#include <stdbool.h>
#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t start); */
#include "../utils/mod_pow.h"     /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

seq_t a(idx_t n) {
    /* n is 1‑based; n ≥ 1 per the problem statement. */
    if (n == 0) return 0;  /* undefined but safe */

    uint64_t target = (uint64_t)n;
    uint64_t count   = 0;
    uint64_t p       = 1;          /* start below the first prime */

    while (count < target) {
        p = next_prime(p);          /* next prime after p */
        if ((p % 7ULL) == 1ULL) {   /* only primes ≡ 1 (mod 7) can fail 3⁷ ≡ 3 */
            seq_t r = mod_pow((seq_t)3, (p - 1ULL) / 7ULL, (seq_t)p);
            if (r != (seq_t)1) {
                count++;
                if (count == target) {
                    return (seq_t)p;
                }
            }
        }
    }
    /* In practice unreachable */
    return 0;
}
