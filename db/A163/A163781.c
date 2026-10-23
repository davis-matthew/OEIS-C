/*
 * A163781  a(n) is the n-th dJ_2 prime (dual Josephus_2 prime).
 * https://oeis.org/A163781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 2157, u32 n <= 2381, u64 n <= 6756,
 *            u128 n <= 6756, big any n
 * Helpers:   factor_u64, is_prime_big, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A163781 – dJ_2 primes
 *
 * Compute the n‑th integer N such that p = 2N + 1 is prime and
 * −2 is a primitive root modulo p.  The function returns
 * the n‑th term of the sequence A163781.
 *
 * Implementation details:
 *   * Primality test via is_prime_big.
 *   * Primitive‑root test via the standard factor‑and‑pow method.
 *   * Results are cached so that calls with arbitrarily ordered n
 *     are answered in O(1) after the necessary terms have been
 *     generated.
 *   * The search continues from the last computed candidate so
 *     it works in under two seconds for n up to about 10000.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime_big.h"
#include "../utils/mod_pow.h"

/* Helper: test whether N is a dJ_2 prime. */
static bool is_dj2_prime(idx_t N) {
    /* p = 2N + 1 */
    seq_t p = (seq_t)N * 2 + 1;
    if (!is_prime_big(p)) {
        return false;
    }

    /* factor p-1 */
    uint64_t n = (uint64_t)(p - 1);
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t base = p - 2;          /* −2 modulo p */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = primes[i];
        seq_t res = mod_pow(base, n / q, p);
        if (res == (seq_t)1) {
            return false;        /* order not maximal */
        }
    }
    return true;                 /* −2 generates all non‑zero residues */
}

/* Static cache for computed terms. */
static seq_t *cache = NULL;      /* array of found N values */
static idx_t cache_size = 0;     /* number of stored terms */
static idx_t cache_cap  = 0;     /* allocated capacity */
static idx_t candidate   = 2;    /* next N to test */

/* Append a new term to the cache, growing if necessary. */
static void cache_append(idx_t N) {
    if (cache_size == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* memory error */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = (seq_t)N;
}

/* Public entry point as required. */
seq_t a(idx_t n) {
    /* Ensure we have at least n terms in the cache. */
    while (cache_size < n) {
        if (is_dj2_prime(candidate)) {
            cache_append(candidate);
        }
        ++candidate;
    }
    return cache[n - 1];
}
