/*
 * A038913  Primes p such that 37 is a square mod p.
 * https://oeis.org/A038913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   init_good_residues
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n-th prime p such that 37 is a quadratic residue modulo p
   (including p = 37).  Computed by incremental prime generation and primality
   testing by trial division against previously found primes.  */

/* The utilities below are provided by the build environment.  */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/init_good_residues.h"

/* Test whether n (uint64_t) is prime, using the list of already found primes. */
static bool is_prime_uint64(uint64_t n, const uint64_t *primes, size_t primes_cnt)
{
    if (n < 2) return false;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Main OEIS sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 */

    /* Static state for memoisation */
    static seq_t *seq_cache = NULL;         /* cached sequence terms */
    static size_t seq_cnt = 0, seq_cap = 0; /* how many cached terms / capacity */

    static uint64_t *prime_cache = NULL;    /* list of primes found so far */
    static size_t prime_cnt = 0, prime_cap = 0;

    static bool good[37];
    static bool good_init = false;
    if (!good_init) {
        init_good_residues(good);
        good_init = true;
    }

    /* Ensure we have computed at least n terms. */
    while (seq_cnt < (size_t)n) {
        /* Find the next prime > last one known. */
        uint64_t candidate = 2;
        if (prime_cnt > 0) candidate = prime_cache[prime_cnt-1] + 1;

        /* Loop until we find the next qualifying prime. */
        while (true) {
            /* Skip even numbers except 2. */
            if (candidate > 2 && (candidate & 1) == 0) {
                ++candidate;
                continue;
            }

            /* Ensure prime_cache is large enough. */
            if (prime_cnt == prime_cap) {
                size_t new_cap = prime_cap == 0 ? 64 : prime_cap * 2;
                uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
                if (!tmp) exit(1);
                prime_cache = tmp;
                prime_cap = new_cap;
            }

            /* Test primality. */
            if (is_prime_uint64(candidate, prime_cache, prime_cnt)) {
                /* Record the new prime. */
                prime_cache[prime_cnt++] = candidate;

                /* Accept the prime if it satisfies the residue condition. */
                if (candidate == 37 || good[candidate % 37U]) {
                    /* Ensure seq_cache has space. */
                    if (seq_cnt == seq_cap) {
                        size_t new_cap = seq_cap == 0 ? 16 : seq_cap * 2;
                        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
                        if (!tmp) exit(1);
                        seq_cache = tmp;
                        seq_cap = new_cap;
                    }
                    seq_cache[seq_cnt++] = (seq_t)candidate;
                    break;             /* advance to next required term */
                }
            }
            ++candidate;
        }
    }

    return seq_cache[(size_t)n - 1];
}
