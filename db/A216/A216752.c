/*
 * A216752  Primes p such that x^42 = -2 has no solution mod p.
 * https://oeis.org/A216752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates primes p such that the congruence
   x^42 = -2 (mod p) has no solution.
   For each prime p > 3, the congruence has a solution iff
   (-2)^((p-1)/gcd(42,p-1)) ≡ 1 (mod p).
   Therefore we select primes for which that expression is not 1. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* static cache of the primes that satisfy the condition */
    static seq_t *primes = NULL;
    static idx_t count = 0;       /* number of stored terms */
    static idx_t capacity = 0;    /* allocated size of primes */
    static uint64_t last_checked = 4; /* last integer examined for primeness */

    /* ensure we have at least n terms stored */
    while (count < n) {
        /* find the next prime > last_checked */
        uint64_t p = next_prime(last_checked);
        last_checked = p;

        /* Compute g = gcd(42, p-1) */
        uint64_t g = 42;
        uint64_t m = p - 1;
        while (m) {
            uint64_t t = m;
            m = g % m;
            g = t;
        }
        /* exponent e = (p-1)/g */
        uint64_t e = (p - 1) / g;

        /* test whether (-2)^(e) ≡ 1 (mod p) */
        seq_t base = (seq_t)(p - 2);          /* (-2) mod p */
        seq_t mod  = (seq_t)p;
        seq_t val  = mod_pow(base, e, mod);
        if (val != (seq_t)1) {                /* no solution, keep the prime */
            /* grow the cache if necessary */
            if (count == capacity) {
                size_t new_cap = capacity ? capacity * 2 : 32;
                seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                if (!tmp) return 0; /* out‑of‑memory: return 0 as sentinel */
                primes   = tmp;
                capacity = new_cap;
            }
            primes[count++] = (seq_t)p;
        }
    }
    /* a(1) corresponds to primes[0] */
    return primes[n - 1];
}
