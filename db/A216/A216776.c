/*
 * A216776  Primes p such that x^62 = -2 has no solution mod p.
 * https://oeis.org/A216776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 882 (2026-11-01)
 * Helpers:   is_prime, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that x^62 = -2 has no solution modulo p.
   Criterion: for a prime p, -2 is a 62nd power residue mod p iff
   (-2)^((p-1)/gcd(62,p-1)) ≡ 1 (mod p).  The sequence consists of
   primes for which this congruence fails. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime.h"
#include "../utils/powmod.h"

/* Greatest‑common divisor for unsigned 64‑bit integers. */
static inline uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset: sequence starts at 1 */

    idx_t found = 0;
    uint64_t p = 5;                /* start with the first odd prime ≥5 */

    while (true) {
        if (is_prime(p)) {
            uint64_t g   = gcd_u64(62ULL, p - 1ULL);
            uint64_t exp = (p - 1ULL) / g;
            seq_t base   = (seq_t)(p - 2ULL);          /* (-2 mod p) */
            seq_t v      = powmod(base, exp, (seq_t)p);

            if (v != (seq_t)1) {               /* no 62nd‑power solution */
                ++found;
                if (found == n) return (seq_t)p;
            }
        }
        p += 2;                            /* skip even numbers */
    }

    return (seq_t)0;  /* unreachable */
}
