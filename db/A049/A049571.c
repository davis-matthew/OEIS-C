/*
 * A049571  Primes p such that x^39 = 2 has a solution mod p.
 * https://oeis.org/A049571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that x^39 = 2 has a solution modulo p.
   Algorithm: generate primes and accept p iff 2^((p-1)/gcd(39,p-1)) ≡ 1 (mod p).
   p = 2 is accepted by definition. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Cache of found primes. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    /* List of primes used for trial division. */
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt = 0, prime_cap = 0;

    /* Next candidate for primality testing (skip 2, which we handle specially). */
    static uint64_t next_candidate = 3;

    /* One-time allocation of the cache and prime list. */
    if (!cache) {
        cache_cap = (size_t)n * 2 + 8;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));

        prime_cap = 16;
        prime_list = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));

        /* Include 2 by default. */
        cache[0] = (seq_t)2;
        cache_len = 1;

        prime_list[0] = 2;
        prime_cnt = 1;
    }

    /* Ensure enough room for the requested element. */
    if (n > cache_cap) {
        size_t new_cap = n * 2;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Return early if already computed. */
    if (n <= cache_len)
        return cache[n-1];

    /* Generate primes until we have `n` qualifying primes. */
    while (cache_len < n) {
        uint64_t p = next_candidate;
        next_candidate += 2;                 /* skip even numbers */

        /* Primality test by trial division using stored primes. */
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t q = prime_list[i];
            if (q * q > p) break;
            if (p % q == 0) { is_prime = false; break; }
        }
        if (!is_prime)
            continue;

        /* Store new prime for future division tests. */
        if (prime_cnt == prime_cap) {
            size_t new_cap = prime_cap * 2;
            prime_list = (uint64_t*)realloc(prime_list, new_cap * sizeof(uint64_t));
            prime_cap = new_cap;
        }
        prime_list[prime_cnt++] = p;

        /* Check the residue condition. */
        seq_t sp = (seq_t)p;
        seq_t p_minus1 = sp - 1;
        seq_t g = gcd((seq_t)39, p_minus1);
        seq_t exponent = p_minus1 / g;

        /* Modular exponentiation: 2^exponent mod p */
        seq_t pow = (seq_t)1;
        seq_t base = (seq_t)2;
        seq_t exp  = exponent;
        seq_t mod  = sp;
        while (exp != 0) {
            if (exp & (seq_t)1) pow = (pow * base) % mod;
            base = (base * base) % mod;
            exp >>= 1;
        }

        if (pow == (seq_t)1)
            cache[cache_len++] = sp;
    }

    return cache[n-1];
}
