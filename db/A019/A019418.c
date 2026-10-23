/*
 * A019418  Primes with primitive root 96.
 * https://oeis.org/A019418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, factorize, is_prime_small, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: Primes with primitive root 96
 *
 * a(n) is the n-th prime p for which 96 is a primitive root modulo p.
 * The algorithm searches successive primes and tests the primitive‑root
 * condition by checking 96^((p-1)/q) mod p ≠ 1 for every distinct prime
 * divisor q of p‑1.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"
#include "../utils/factorize.h"
#include "../utils/mod_pow.h"
#include "../utils/ensure_capacity.h"

/* Return the nth prime p such that 96 is a primitive root modulo p. */
seq_t a(idx_t n) {
    /* Static cache of found values. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Static state for continued search. */
    static uint64_t next_candidate = 5;     /* start from 5 (prime) */
    static uint64_t found = 0;              /* number of primes found */

    /* Grow cache when needed. */
    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* Temporary arrays for factorization (distinct prime factors only). */
    uint64_t pf[64];
    int exps[64];

    /* Main search loop. */
    while (found < (size_t)n) {
        /* Skip even numbers (except 2), but start from 5 so only odds. */
        uint64_t p = next_candidate;

        /* Check primality. */
        if (is_prime_small((seq_t)p) && p > 3) {
            /* 96 must be coprime to p; skip p dividing 96 (only 2,3). */
            if (p != 2 && p != 3) {
                /* Factor p-1. */
                uint64_t m = p - 1;
                size_t fc = factorize(m, pf, exps);

                /* Test primitive-root condition. */
                bool ok = true;
                for (size_t i = 0; i < fc; ++i) {
                    uint64_t q = pf[i];
                    seq_t res = mod_pow((seq_t)96, m / q, (seq_t)p);
                    if (res == (seq_t)1) {
                        ok = false;
                        break;
                    }
                }

                if (ok) {
                    /* Store found prime in cache. */
                    if (cache_len == cache_cap) {
                        /* Grow cache by a factor of 2. */
                        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                        ensure_capacity(new_cap, &cache_cap,
                                        (void **)&cache, sizeof(seq_t));
                    }
                    cache[cache_len++] = (seq_t)p;
                    ++found;
                }
            }
        }

        /* Advance to next candidate prime (skip even numbers). */
        next_candidate += 2;
    }

    return cache[n - 1];
}
