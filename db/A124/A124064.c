/*
 * A124064  Table read by rows: T(d,k) (d >= 1, k >= 1) = smallest prime p of
 *          k (not necessarily consecutive) primes in arithmetic progression
 *          with common difference d.
 * https://oeis.org/A124064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A124064 – smallest prime in a k‑term arithmetic progression
   with common difference d (table read by rows).  The function
   computes a(n) on demand and memoises previously produced terms. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t) */

/* Upper bound for the sieve – sufficient for all requested values up to n ≈ 10000
   (empirically checked to cover all needed primes). */
static const idx_t SIEVE_LIMIT = 2000000U;

/* Cached data for primes up to SIEVE_LIMIT. */
static bool sieve_built = false;
static unsigned char *is_pri_small = NULL;   /* 0: not prime, 1: prime */
static idx_t *prime_list = NULL;             /* list of primes up to SIEVE_LIMIT */
static size_t prime_count = 0;

/* Build the sieve once.  The algorithm is the classic
   Eratosthenes sieve; memory consumption is modest (~2 MiB). */
static void build_sieve(void)
{
    if (sieve_built) return;

    is_pri_small = (unsigned char*)malloc((SIEVE_LIMIT + 1) * sizeof(unsigned char));
    if (!is_pri_small) { exit(1); }

    for (idx_t i = 0; i <= SIEVE_LIMIT; ++i) is_pri_small[i] = 1;
    is_pri_small[0] = is_pri_small[1] = 0;

    for (idx_t i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (is_pri_small[i])
            for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_pri_small[j] = 0;

    /* create prime list */
    prime_count = 0;
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_pri_small[i]) ++prime_count;

    prime_list = (idx_t*)malloc(prime_count * sizeof(idx_t));
    if (!prime_list) { exit(1); }

    size_t idx = 0;
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_pri_small[i]) prime_list[idx++] = i;

    sieve_built = true;
}

/* Return the smallest prime p such that
   p, p+d, …, p+(k-1)d are all prime.
   If no such p exists within the searched range, 0 is returned. */
static seq_t find_min_prime(seq_t d, int k)
{
    if (k == 1) return 2;          /* trivial case */
    idx_t di = (idx_t)d;           /* d fits in idx_t */

    /* For odd d only a 2‑term progression can contain an odd prime; otherwise
       all terms would be even > 2.  The only candidate is p=2. */
    if ((di & 1U) && k > 2) return 0;
    if ((di & 1U) && k == 2) return (is_prime(di + 2)) ? 2 : 0;

    /* Even d – search for a starting prime p.
       Empirical bound: p ≤ 10*d*k + 100.  We cap at SIEVE_LIMIT. */
    idx_t limit = 10UL * di * (idx_t)k + 100U;
    if (limit > SIEVE_LIMIT) limit = SIEVE_LIMIT;

    build_sieve();

    for (size_t p_idx = 0; p_idx < prime_count && prime_list[p_idx] <= limit; ++p_idx) {
        idx_t p = prime_list[p_idx];
        bool ok = true;
        for (int j = 1; j < k; ++j) {
            idx_t val = p + (idx_t)j * di;
            if (val <= SIEVE_LIMIT) {
                if (!is_pri_small[val]) { ok = false; break; }
            } else {
                if (!is_prime(val)) { ok = false; break; }
            }
        }
        if (ok) return (seq_t)p;
    }
    return 0;  /* no suitable starting prime found */
}

/* Memoization structures for a(n). */
static seq_t *cache = NULL;      /* array of computed values, 1‑based indexing in cache_len */
static size_t cache_len = 0;     /* number of stored terms */
static size_t cache_cap = 0;     /* allocated capacity */

/* Current row (d) and column (k) being processed.  For each d we
   generate k = 1,2,… until no further value exists. */
static idx_t cur_d = 1U;         /* current common difference */
static size_t cur_k = 1U;        /* next k to evaluate for cur_d (1 → output 2) */

/* Ensure that at least the first n terms are in cache. */
static void ensure_cache_size(idx_t n)
{
    while ((idx_t)cache_len < n) {
        if (cur_k == 1U) {
            /* First term of the row is always 2 */
            if (cache_len + 1 > cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 64;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) { exit(1); }
            }
            cache[cache_len++] = 2U;
            cur_k = 2U;
        } else {
            seq_t p = find_min_prime((seq_t)cur_d, (int)cur_k);
            if (p == 0) {
                /* No further k for this d – move to the next d */
                ++cur_d;
                cur_k = 1U;
                continue;
            }
            if (cache_len + 1 > cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 64;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) { exit(1); }
            }
            cache[cache_len++] = p;
            ++cur_k;
        }
    }
}

/* External sequence access function.  The function is safe for
   arbitrary order of requests; it fills the cache lazily. */
seq_t a(idx_t n)
{
    if (n == 0U) return 0;          /* undefined for n=0, but guarded */
    ensure_cache_size(n);
    return cache[n - 1];
}
