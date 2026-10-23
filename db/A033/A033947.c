/*
 * A033947  Smallest primitive root (in absolute value) of n-th prime.
 * https://oeis.org/A033947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 32, i16 n <= 3822, i32 n <= 4883, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: For the n‑th prime p, find the primitive root g of p
 * with minimal absolute value.  If both +k and –k are primitive roots
 * with that minimal |k|, return +k.  The function builds a small
 * prime table up to a fixed limit (200 000) and caches computed
 * sequence values.  Complexity per call is O(1) on average after
 * the initial sieve. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* Pool of all primes up to the chosen limit. */
static uint64_t *prime_dual = NULL;      /* primes of build_primes_upto */
static size_t prime_dual_cnt = 0;

/* First n primes, stored as uint32_t for quick indexing. */
static uint32_t *prime_list = NULL;
static size_t prime_list_len = 0;

/* Cache of already computed sequence values. */
static seq_t *seq_cache = NULL;
static size_t seq_cache_len = 0;

/* Ensure the pool of primes is available. */
static void init_prime_pool(void)
{
    if (prime_dual != NULL)
        return;
    /* Choose a limit that exceeds the 10 000th prime (~104 729). */
    build_primes_upto(200000, &prime_dual, &prime_dual_cnt);
}

/* Ensure that prime_list contains at least n first primes. */
static void ensure_prime_list(size_t n)
{
    if (prime_list_len >= n)
        return;
    uint32_t *tmp = (uint32_t *)realloc(prime_list, n * sizeof(uint32_t));
    prime_list = tmp;
    for (size_t i = prime_list_len; i < n; ++i)
        prime_list[i] = (uint32_t)prime_dual[i];
    prime_list_len = n;
}

/* Fast modular exponentiation with seq_t operands. */
static seq_t modpow_seq(seq_t base, uint64_t exp, seq_t mod)
{
    seq_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

/* Test whether a modulo p is a primitive root (p is prime).  The
 * array “factors” contains the distinct prime factors of p‑1. */
static bool is_primitive_uint64(uint64_t a, uint64_t p,
                                const uint64_t *factors, size_t fc)
{
    seq_t mod = (seq_t)p;
    seq_t base = (seq_t)a % mod;
    for (size_t i = 0; i < fc; ++i) {
        uint64_t exp = (p - 1) / factors[i];
        if (modpow_seq(base, exp, mod) == 1)
            return false;
    }
    return true;
}

/* Compute a(n) and store it in the cache.  idx is 1‑based. */
static void compute_value(idx_t idx)
{
    uint64_t p = prime_list[idx - 1];      /* nth prime */

    /* Factorize p-1 into distinct prime factors. */
    uint64_t factors[64];
    size_t fact_cnt = 0;
    uint64_t m = p - 1;
    for (size_t i = 0; i < prime_dual_cnt && prime_dual[i] * prime_dual[i] <= m; ++i) {
        uint64_t prime = prime_dual[i];
        if (m % prime == 0) {
            factors[fact_cnt++] = prime;
            while (m % prime == 0)
                m /= prime;
        }
    }
    if (m > 1)
        factors[fact_cnt++] = m;

    uint64_t limit = p / 2;
    for (uint64_t d = 1; d <= limit; ++d) {
        if (is_primitive_uint64(d, p, factors, fact_cnt)) {
            seq_cache[idx - 1] = (seq_t)d;
            return;
        }
        if (is_primitive_uint64(p - d, p, factors, fact_cnt)) {
            seq_cache[idx - 1] = -(seq_t)d;
            return;
        }
    }
    /* Should never reach here for a prime p. */
    seq_cache[idx - 1] = (seq_t)0;
}

/* Main sequence function required by the framework. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for 0, return 0 as placeholder */

    init_prime_pool();
    ensure_prime_list((size_t)n);

    while (seq_cache_len < (size_t)n) {
        seq_t *tmp = (seq_t *)realloc(seq_cache,
                                      (seq_cache_len + 1) * sizeof(seq_t));
        seq_cache = tmp;
        compute_value(seq_cache_len + 1);
        ++seq_cache_len;
    }
    return seq_cache[n - 1];
}
