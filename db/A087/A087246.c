/*
 * A087246  Squarefree deficient numbers.
 * https://oeis.org/A087246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Squarefree deficient numbers (A087246)
 * a(n) is the n‑th positive integer that is squarefree
 * and has σ(n) < 2 n.  Off‑set = 1 (a(1)=1).
 *
 * The implementation uses a linear sieve to factor candidates.
 * Cache of already‑computed terms is kept in a 64‑bit array;
 * the value is cast to seq_t on return, which works for all
 * integer seq_t types (unsigned integers or arbitrary‑precision
 * integers that can be constructed from a 64‑bit integer).
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/*** static state ***/
static uint64_t sieve_limit = 0;          /* current sieve upper bound   */
static uint32_t *spf = NULL;              /* smallest prime factor table */
static size_t spf_len = 0;                /* length of spf (sieve_limit+1) */

static uint32_t *prime_list = NULL;       /* list of primes up to sieve_limit */
static size_t prime_cnt = 0;
static size_t prime_cap = 0;

/* cache of computed terms (stored as 64‑bit integers) */
static uint64_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/*** helper: build linear sieve up to 'limit' (inclusive) ***/
static void build_sieve(uint64_t limit)
{
    /* resize spf array */
    spf = (uint32_t *)realloc(spf, (limit + 1) * sizeof(uint32_t));
    if (!spf) exit(EXIT_FAILURE);
    spf_len = limit + 1;
    for (size_t i = 0; i < spf_len; ++i) spf[i] = 0;

    /* ensure prime list capacity */
    if (prime_cap < limit + 10) {
        prime_cap = limit + 10;
        prime_list = (uint32_t *)realloc(prime_list, prime_cap * sizeof(uint32_t));
        if (!prime_list) exit(EXIT_FAILURE);
    }
    prime_cnt = 0;

    for (uint64_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                /* i is prime */
            spf[i] = (uint32_t)i;
            if (prime_cnt < prime_cap) {
                prime_list[prime_cnt++] = (uint32_t)i;
            }
        }
        uint32_t fi = spf[i];
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = prime_list[j];
            uint64_t v = i * (uint64_t)p;
            if (v > limit) break;
            spf[v] = p;
            if (p == fi) break;
        }
    }
}

/*** helper: expand sieve to cover at least 'n' ***/
static void ensure_sieve(uint64_t n)
{
    if (n <= sieve_limit) return;
    uint64_t new_limit = (sieve_limit > 1) ? sieve_limit : 2;
    while (new_limit < n) {
        new_limit <<= 1;
    }
    build_sieve(new_limit);
    sieve_limit = new_limit;
}

/*** helper: check squarefree & deficient ***/
static bool is_squarefree_deficient(uint64_t n)
{
    if (n == 1) return true; /* 1 is squarefree and σ(1)=1<2 */

    __int128 prod = 1;
    uint64_t x = n;
    while (x > 1) {
        uint32_t p = spf[x];
        uint64_t exp = 0;
        while (x % p == 0) {
            x /= p;
            ++exp;
            if (exp > 1) return false;    /* not squarefree */
        }
        prod *= (__int128)(p) + 1;
        if (prod >= (__int128)2 * n) return false; /* not deficient */
    }
    return prod < (__int128)2 * n;
}

/*** helper: resize cache for at least 'need' entries ***/
static void ensure_cache(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap > 0 ? cache_cap : 16;
    while (new_cap < need) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(cache, new_cap * sizeof(uint64_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/*** main sequence function ***/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined offset */

    /* initialisation of sieve & cache */
    if (sieve_limit == 0) {
        ensure_sieve(2);
    }
    if (cache == NULL) {
        cache = (uint64_t *)malloc(16 * sizeof(uint64_t));
        if (!cache) exit(EXIT_FAILURE);
        cache_cap = 16;
    }

    static uint64_t next_candidate = 1;     /* next integer to test */

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        ensure_sieve(next_candidate);
        if (is_squarefree_deficient(next_candidate)) {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }

    return (seq_t)cache[n - 1];
}
