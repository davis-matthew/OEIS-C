/*
 * A195743  Number of distinct residues of prime(k)^n (mod n), prime(k) <= n.
 * https://oeis.org/A195743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 486, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = number of distinct residues of prime(k)^n (mod n)
   for all primes p ≤ n.  The value is obtained by iterating over the
   primes up to n, computing p^n mod n with binary exponentiation,
   and counting distinct residues. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* local, non‑static helper: binary modular exponentiation */
static seq_t mod_pow_local(seq_t base, idx_t exp, seq_t mod) {
    seq_t result = 1 % mod;
    seq_t b = base % mod;
    while (exp) {
        if (exp & 1) result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return result;
}

/* local cache of primes up to a certain limit */
static uint64_t *prime_cache = NULL;
static size_t prime_cache_cnt = 0;
static idx_t prime_cache_limit = 0;

/* ensure we have primes up to n; rebuild cache if necessary */
static void ensure_primes(idx_t n) {
    if (prime_cache_limit >= n) return;

    free(prime_cache);
    prime_cache_limit = n;
    prime_cache_cnt = 0;
    prime_cache = (uint64_t *)malloc((size_t)n * sizeof(uint64_t));
    if (!prime_cache) exit(1);

    /* simple sieve of Eratosthenes */
    bool *is_composite = (bool *)calloc((size_t)n + 1, sizeof(bool));
    if (!is_composite) exit(1);

    for (size_t i = 2; i <= (size_t)n; ++i) {
        if (!is_composite[i]) {
            prime_cache[prime_cache_cnt++] = (uint64_t)i;
            if (i * i <= (size_t)n) {
                for (size_t j = i * i; j <= (size_t)n; j += i) {
                    is_composite[j] = true;
                }
            }
        }
    }
    free(is_composite);
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n == 1) return 0;          /* a(1) = 0 by definition */

    ensure_primes(n);

    /* allocate boolean array to mark residues seen so far */
    unsigned char *seen = (unsigned char *)calloc((size_t)n, sizeof(unsigned char));
    if (!seen) return 0;           /* out of memory: return 0 */
    size_t distinct = 0;

    for (size_t i = 0; i < prime_cache_cnt; ++i) {
        uint64_t p = prime_cache[i];
        if (p > (uint64_t)n) break;

        seq_t residue = mod_pow_local((seq_t)p, (idx_t)n, (seq_t)n);
        size_t idx = (size_t)residue;
        if (!seen[idx]) {
            seen[idx] = 1;
            ++distinct;
        }
    }

    free(seen);
    return distinct;
}
