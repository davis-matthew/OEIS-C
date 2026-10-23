/*
 * A049582  Primes p such that x^50 = 2 has a solution mod p.
 * https://oeis.org/A049582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the n-th prime p such that 2 is a 50-th power residue modulo p. */
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)2;          /* first term is 2 */
    idx_t count = 1;                      /* we already counted prime 2 */

    for (idx_t p = 3; ; p += 2) {           /* iterate over odd candidates */
        if (!is_prime(p)) continue;         /* skip non‑primes */

        seq_t g = gcd((seq_t)50, (seq_t)(p - 1));
        seq_t exp = (seq_t)(p - 1) / g;      /* exponent (p‑1)/gcd(50,p‑1) */
        seq_t val = mod_pow((seq_t)2, (uint64_t)exp, (seq_t)p);

        if (val == (seq_t)1) {              /* 2 is a 50‑th power residue */
            ++count;
            if ((idx_t)count == n) return (seq_t)p;
        }
    }
}
