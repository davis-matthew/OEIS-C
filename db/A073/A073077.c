/*
 * A073077  Numbers k that divide C(2k,k) and C(4k,2k).
 * https://oeis.org/A073077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 2663, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth member of OEIS A073077, the smallest k ≥ 1 such that
   k divides both C(2k,k) and C(4k,2k).  The implementation tests
   successive integers k, checks prime‐factor exponents with Legendre’s
   formula, and caches all found k’s so repeated calls are fast. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- Prime table ---------- */
#define PRIME_LIMIT 2000000U          /* upper bound for precomputed primes */

static uint32_t *prime_list   = NULL; /* array of primes */
static size_t   prime_count   = 0;    /* number of primes */
static bool     primes_inited = false;

static bool init_primes(void)
{
    if (primes_inited) return true;

    size_t limit = PRIME_LIMIT;
    prime_list = (uint32_t *)malloc(limit * sizeof(uint32_t));
    if (!prime_list) return false;

    bool *is_composite = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_composite) {
        free(prime_list);
        return false;
    }

    size_t cnt = 0;
    for (size_t p = 2; p <= limit; ++p) {
        if (!is_composite[p]) {
            if (cnt >= limit) break;             /* safety */
            prime_list[cnt++] = (uint32_t)p;
            if (p * p <= limit) {
                for (size_t q = p * p; q <= limit; q += p)
                    is_composite[q] = true;
            }
        }
    }

    prime_count = cnt;
    free(is_composite);
    primes_inited = true;
    return true;
}

/* ---------- Legendre exponent in factorial ---------- */
static idx_t legendre_factorial(idx_t n, idx_t p)
{
    idx_t exp = 0;
    while (n) {
        n /= p;
        exp += n;
    }
    return exp;
}

/* ---------- Test if k satisfies the divisibility conditions ---------- */
static bool good_k(idx_t k)
{
    if (!init_primes()) return false; /* cannot proceed without primes */

    idx_t rem = k;
    for (size_t i = 0; i < prime_count &&
                        (idx_t)prime_list[i] * prime_list[i] <= rem; ++i) {
        uint32_t p = prime_list[i];
        if (rem % p == 0) {
            idx_t exp_k = 0;
            while (rem % p == 0) {
                rem /= p;
                ++exp_k;
            }

            idx_t e1 = legendre_factorial(2 * k, p) - 2 * legendre_factorial(k, p);
            if (e1 < exp_k) return false;

            idx_t e2 = legendre_factorial(4 * k, p) - 2 * legendre_factorial(2 * k, p);
            if (e2 < exp_k) return false;
        }
    }

    /* remaining prime factor (if any) */
    if (rem > 1) {
        uint32_t p = (uint32_t)rem;
        idx_t exp_k = 1;

        idx_t e1 = legendre_factorial(2 * k, p) - 2 * legendre_factorial(k, p);
        if (e1 < exp_k) return false;

        idx_t e2 = legendre_factorial(4 * k, p) - 2 * legendre_factorial(2 * k, p);
        if (e2 < exp_k) return false;
    }

    return true;
}

/* ---------- Cache for sequence values ---------- */
static idx_t *cache_idx   = NULL; /* cached k's, stored as idx_t */
static size_t  cache_cap  = 0;
static size_t  cache_len  = 0;
static idx_t   last_k     = 0;    /* largest k already tested */

/* ---------- Main function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, but safe */

    /* initialise cache once */
    if (!cache_idx) {
        cache_cap = 64;
        cache_idx = (idx_t *)malloc(cache_cap * sizeof(idx_t));
        if (!cache_idx) return (seq_t)0; /* allocation failed */
        cache_len = 0;
        last_k = 0;
    }

    /* generate terms until we have n entries */
    while (cache_len < n) {
        ++last_k;                          /* candidate k */
        if (good_k(last_k)) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                idx_t *tmp = (idx_t *)realloc(cache_idx, cache_cap * sizeof(idx_t));
                if (!tmp) return (seq_t)0; /* allocation failed */
                cache_idx = tmp;
            }
            cache_idx[cache_len++] = last_k;
        }
    }

    return (seq_t)cache_idx[n - 1];
}
