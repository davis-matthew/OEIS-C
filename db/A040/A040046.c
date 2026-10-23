/*
 * A040046  Primes p such that x^3 = 6 has a solution mod p.
 * https://oeis.org/A040046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime p for which the congruence x³ = 6 (mod p)
 *            has a solution. This is equivalent to: for p | 6 a solution
 *            exists trivially; otherwise 6^((p-1)/gcd(3,p-1)) ≡ 1 (mod p)
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use helpers from the utils library */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"       /* bool is_prime(idx_t x) */
#include "../utils/mod_pow.h"        /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod) */

static seq_t *goodprimes = NULL;  /* cache of qualifying primes */
static size_t good_len = 0;       /* number of primes stored */
static size_t good_cap = 0;       /* capacity of goodprimes */
static idx_t next_candidate = 2;  /* next integer to test for primeness */

/* append a prime to the cache, resizing if necessary */
static void append_good(idx_t p)
{
    if (good_len == good_cap) {
        size_t new_cap = good_cap == 0 ? 16 : good_cap * 2;
        seq_t *tmp = (seq_t *)realloc(goodprimes, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        goodprimes = tmp;
        good_cap = new_cap;
    }
    goodprimes[good_len++] = (seq_t)p;
}

/* test whether p satisfies the cubic residue condition */
static inline bool good_prime(idx_t p)
{
    if (p == 2 || p == 3) return true;
    idx_t d = ((p - 1) % 3 == 0) ? 3 : 1;
    idx_t exp = (p - 1) / d;
    seq_t res = mod_pow((seq_t)6, exp, (seq_t)p);
    return res == (seq_t)1;
}

/* ensure that the cache contains at least n qualifying primes */
static void ensure_good_prime_count(idx_t n)
{
    if (n == 0 || good_len >= n) return;

    while (good_len < n) {
        if (is_prime(next_candidate)) {
            if (good_prime(next_candidate))
                append_good(next_candidate);
        }
        /* advance to next candidate: after 2 use odd increments */
        if (next_candidate == 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out-of-range guard */
    ensure_good_prime_count(n);
    return goodprimes[n - 1];
}
