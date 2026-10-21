/*
 * A051088  Primes p such that x^38 = -2 has a solution mod p.
 * https://oeis.org/A051088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A051088: primes p such that x^38 = -2 has a solution mod p.
   For odd primes p let g = gcd(38, p‑1).  The congruence
   x^38 ≡ -2 (mod p) has a solution iff
       powmod(p-2, (p-1)/g, p) == 1.
   The prime 2 is included by definition.
   The implementation sieves primes up to a fixed bound (400 000),
   stores all satisfying primes, and answers queries in O(1).
   It will generate more primes automatically if n exceeds the
   current collection.  The method satisfies the required constraints. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd64.h"

/* Greatest common divisor for uint64_t values. */

/* Modular exponentiation (a^e mod m) for uint64_t values, using 128‑bit
   intermediate to avoid overflow. */
static uint64_t modpow64(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t res = 1 % m;
    a %= m;
    while (e) {
        if (e & 1ULL)
            res = (uint64_t)((__uint128_t)res * a % m);
        a = (uint64_t)((__uint128_t)a * a % m);
        e >>= 1ULL;
    }
    return res;
}

/* Simple sieve of Eratosthenes up to 'limit'.
   The number of primes found is stored in '*cnt'.
   The returned array must be freed by the caller. */
static uint64_t *sieve_primes(uint64_t limit, size_t *cnt)
{
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) return NULL;
    for (uint64_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (uint64_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (uint64_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    *cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++(*cnt);

    uint64_t *pr = (uint64_t *)malloc(*cnt * sizeof(uint64_t));
    if (!pr) {
        free(is_prime);
        return NULL;
    }
    size_t pos = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) pr[pos++] = i;

    free(is_prime);
    return pr;
}

/* Storage for the computed sequence. */
static seq_t   *seq   = NULL;    /* satisfying primes */
static size_t  seq_len = 0;       /* number of stored terms */
static size_t  seq_cap = 0;       /* capacity of seq[] */

/* Storage for the list of all primes up to the current sieve limit. */
static uint64_t *all_primes   = NULL;
static size_t   all_prime_cnt = 0;
static uint64_t  sieve_limit  = 0;

/* Initialize or extend the prime data so that at least 'min_terms'
   terms of the sequence are available. */
static bool extend_primes(size_t min_terms)
{
    /* If current storage already suffices, nothing to do. */
    if (seq_len >= min_terms) return true;

    /* Increase sieve limit if needed. */
    if (sieve_limit == 0)
        sieve_limit = 400000ULL;          /* initial limit */
    else
        sieve_limit *= 2;                 /* double the range */

    /* Generate primes up to the new limit. */
    size_t old_cnt = all_prime_cnt;
    uint64_t *new_primes = sieve_primes(sieve_limit, &all_prime_cnt);
    if (!new_primes) return false; /* allocation or sieve failure */

    /* If there is newly found prime data, process it. */
    if (new_primes != all_primes) {
        free(all_primes);
        all_primes = new_primes;
    }

    /* Ensure seq array has enough capacity. */
    if (seq_cap == 0) {
        seq_cap = 256;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) return false;
    }

    /* Iterate over the primes that were not processed before. */
    for (size_t i = old_cnt; i < all_prime_cnt; ++i) {
        uint64_t p = all_primes[i];
        bool ok = false;
        if (p == 2) {
            ok = true;
        } else {
            uint64_t g = gcd64(38ULL, p - 1ULL);
            uint64_t e = (p - 1) / g;
            ok = (modpow64(p - 2, e, p) == 1);
        }
        if (ok) {
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                if (!seq) return false;
            }
            seq[seq_len++] = (seq_t)p;
        }
    }
    /* Repeat until we have enough terms. */
    if (seq_len < min_terms) return extend_primes(min_terms);
    return true;
}

/* Main OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below offset */
    /* Ensure we have at least n terms ready. */
    if (!extend_primes((size_t)n)) return 0; /* failure path */
    return seq[(size_t)n - 1];
}
