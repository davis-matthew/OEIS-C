/*
 * A137791  Number of ways to write n as sum of two positive numbers having no
 *          prime gaps in their factorization.
 * https://oeis.org/A137791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ (x,y) | x + y = n , 1 ≤ x ≤ y , both x and y have no
 *            prime gaps in their factorizations }
 * Range:     u8 n <= 5459, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_spf.h"
#include "../utils/build_primes.h"

/* Static caches shared by all calls */
static idx_t *spf           = NULL; /* smallest prime factor array */
static size_t spf_size       = 0;    /* allocated size of spf  */
static idx_t *primes        = NULL; /* list of primes           */
static size_t primes_cnt    = 0;    /* number of primes         */
static idx_t *prime_index   = NULL; /* prime → index+1 map      */
static idx_t prime_index_size = 0;  /* allocated size of prime_index */
static bool *good           = NULL; /* cache for gap‑free numbers */
static idx_t good_max       = 0;    /* highest n for which good[] is ready */

/* Test membership of a prime in the precomputed prime list.
   primes are 1‑based indexed in prime_index (0 means not a prime). */
static inline bool is_prime_cached(idx_t p)
{
    return p < prime_index_size && prime_index[p] != 0;
}

/* Return true iff x is gap‑free using the global SPF array and prime_index. */
static bool is_gap_free(idx_t x)
{
    if (x == 1) return true;
    idx_t prev_prime = 0;
    idx_t prev_idx   = 0;
    while (x > 1) {
        idx_t p = spf[x];
        if (p == 0) p = x; /* x itself is prime */
        if (p != prev_prime) {
            if (!is_prime_cached(p)) return false;          /* safety */
            idx_t cur_idx = prime_index[p];
            if (prev_prime != 0 && cur_idx != prev_idx + 1) return false;
            prev_prime = p;
            prev_idx = cur_idx;
        }
        /* divide all occurrences of p */
        do { x /= p; } while (x % p == 0);
    }
    return true;
}

/* Extend all precomputed tables to cover numbers up to new_limit (inclusive). */
static void extend_cache(idx_t new_limit)
{
    if (new_limit <= good_max) return;          /* already covered */

    /* Rebuild SPF table for [0..new_limit] */
    free(spf);
    spf = NULL;
    build_spf(new_limit, &spf);
    spf_size = (size_t)new_limit + 1;

    /* Rebuild prime list and prime_index map */
    free(primes);
    primes = NULL;
    free(prime_index);
    prime_index = NULL;
    build_primes(new_limit, &primes, &primes_cnt);
    prime_index_size = (size_t)new_limit + 1;
    prime_index = (idx_t *)calloc(prime_index_size, sizeof(idx_t));
    for (size_t i = 0; i < primes_cnt; ++i)
        prime_index[primes[i]] = (idx_t)(i + 1);  /* index+1 to avoid zero */

    /* Extend good array */
    good = (bool *)realloc(good, (sizeof(bool)) * (new_limit + 1));
    for (idx_t i = good_max + 1; i <= new_limit; ++i)
        good[i] = is_gap_free(i);

    good_max = new_limit;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined below offset */

    /* Ensure cache covers n */
    extend_cache(n);

    seq_t cnt = 0;
    for (idx_t a_val = 1; a_val <= n / 2; ++a_val) {
        if (good[a_val] && good[n - a_val]) ++cnt;
    }
    return cnt;
}
