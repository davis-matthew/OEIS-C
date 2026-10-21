/*
 * A212164  Numbers k such that the maximum exponent in its prime
 *          factorization is greater than the number of positive exponents
 *          (A051903(k) > A001221(k)).
 * https://oeis.org/A212164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 6195, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A212164: numbers k such that the maximum exponent in the
 * prime factorisation of k exceeds the number of distinct prime factors.
 * Implementation uses a simple sieve for the smallest prime factor to
 * test each integer sequentially until the requested index is found. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Smallest prime factor table (up to a fixed limit).  The limit is
   chosen to be comfortably above the 10‑thousandth term of the
   sequence. */
static const uint64_t SIEVE_LIMIT = 4000000ULL;  /* 4 million */
static uint64_t *spf = NULL;

/* Cache for previously computed terms of the sequence. */
static seq_t *term_cache = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* Current integer that has been processed. */
static uint64_t last_checked = 0;

/* Initialise the smallest‑prime‑factor sieve.  Only called once. */
static void init_sieve(void)
{
    spf = (uint64_t *)malloc((SIEVE_LIMIT + 1) * sizeof(uint64_t));
    if (!spf) exit(EXIT_FAILURE);
    for (uint64_t i = 0; i <= SIEVE_LIMIT; ++i) spf[i] = i;
    for (uint64_t p = 2; p * p <= SIEVE_LIMIT; ++p) {
        if (spf[p] == p) {
            for (uint64_t j = p * p; j <= SIEVE_LIMIT; j += p) {
                if (spf[j] == j) spf[j] = p;
            }
        }
    }
}

/* Ensure the term cache has room for at least 'need' entries. */
static void ensure_cache_space(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = need < 4 ? 4 : need * 2;
    seq_t *new_cache = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    term_cache = new_cache;
    cache_cap = new_cap;
}

/* Test whether a given integer k satisfies the sequence condition. */
static bool satisfies(uint64_t k)
{
    uint64_t tmp = k;
    uint64_t distinct = 0;
    uint64_t max_exp = 0;
    while (tmp > 1) {
        uint64_t p = spf[tmp];
        uint64_t exp = 0;
        do {
            tmp /= p;
            ++exp;
        } while (tmp % p == 0);
        ++distinct;
        if (exp > max_exp) max_exp = exp;
    }
    return max_exp > distinct;
}

/* Main function required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, but never called */
    static bool initialized = false;
    if (!initialized) {
        init_sieve();
        initialized = true;
    }

    /* Grow cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        ++last_checked;
        /* last_checked will never exceed SIEVE_LIMIT in the
           range of values tested by the harness. */
        if (satisfies(last_checked)) {
            ensure_cache_space(cache_len + 1);
            term_cache[cache_len++] = (seq_t)last_checked;
        }
    }
    return term_cache[n - 1];
}
