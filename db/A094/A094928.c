/*
 * A094928  Let p = n-th prime == 1 mod 8 (A007519); a(n) = smallest prime q
 *          such that p is not a square mod q.
 * https://oeis.org/A094928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the smallest prime q such that the n-th prime p ≡ 1 (mod
 *            8) has Legendre symbol (p|q) = –1. The sequence is indexed from
 *            1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/jacobi_symbol.h"   /* declares: int jacobi_symbol(uint64_t a, uint64_t n); */

static bool initialized = false;
static uint64_t *primes_all      = NULL;   /* all primes up to LIMIT */
static size_t  primes_all_cnt   = 0;
static uint64_t *primes_1mod8   = NULL;   /* primes ≡ 1 (mod 8) */
static size_t  primes_1mod8_cnt = 0;

/* cache of computed a(n) */
static seq_t *a_cache      = NULL;
static idx_t  a_cache_size = 0;           /* number of cached entries */

/* sieve up to LIMIT once */
static void init_sieve(void)
{
    if (initialized) return;
    const uint64_t LIMIT = 1200000ULL;    /* enough for n=10000 */
    bool *is_prime = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    for (uint64_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (uint64_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }
    /* first estimate capacity */
    size_t capacity = LIMIT / 10;
    primes_all      = (uint64_t*)malloc(capacity * sizeof(uint64_t));
    primes_1mod8    = (uint64_t*)malloc(capacity * sizeof(uint64_t));
    for (uint64_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) {
            if (primes_all_cnt == capacity) {
                capacity *= 2;
                primes_all = (uint64_t*)realloc(primes_all, capacity * sizeof(uint64_t));
            }
            primes_all[primes_all_cnt++] = i;
            if (i % 8 == 1) {
                if (primes_1mod8_cnt == capacity) {
                    capacity *= 2;
                    primes_1mod8 = (uint64_t*)realloc(primes_1mod8, capacity * sizeof(uint64_t));
                }
                primes_1mod8[primes_1mod8_cnt++] = i;
            }
        }
    }
    free(is_prime);
    initialized = true;
}

/* ensure cache large enough for at least n entries, computing missing ones */
static void extend_cache(idx_t n)
{
    if (a_cache_size >= n) return;
    /* enlarge cache */
    size_t new_cap = a_cache_size == 0 ? 8 : a_cache_size * 2;
    while (new_cap < n) new_cap *= 2;
    a_cache = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
    /* compute missing values */
    for (idx_t k = a_cache_size; k < n; ++k) {
        /* n is 1‑based */
        uint64_t p = primes_1mod8[k];          /* n-th prime ≡1 mod 8 */
        seq_t q_found = 0;
        for (size_t idx = 0; idx < primes_all_cnt; ++idx) {
            uint64_t q = primes_all[idx];
            if (q == p) continue;  /* skip q = p; Legendre symbol is 0 */
            if (q == 2) continue;  /* even prime never gives –1 Legendre symbol for odd p */
            int leg = jacobi_symbol(p, q);   /* ≡ (p|q) because q is prime */
            if (leg == -1) { q_found = (seq_t)q; break; }
        }
        a_cache[k] = q_found;
    }
    a_cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* not used for indexed sequence but safe */
    init_sieve();
    extend_cache(n);
    return a_cache[n-1];       /* convert 1‑based n to 0‑based index */
}
