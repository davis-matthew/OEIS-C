/*
 * A064409  Positive even numbers not of the form A001359(i) + A006512(j) for
 *          integers i and j.
 * https://oeis.org/A064409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive even number not equal to
 *            A001359(i)+A006512(j)
 * Range:     u8 n <= 70, u16 n <= 9887, u32 n <= 9887, u64 n <= 9887,
 *            u128 n <= 9887, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* static cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_sz  = 0;

/* twin prime lists */
static uint64_t *twin_low  = NULL;  /* members of A001359 */
static uint64_t *twin_high = NULL;  /* members of A006512 */
static size_t    twin_cnt  = 0;

/* representable sums */
static bool *rep      = NULL;
static uint64_t rep_limit = 0;

/* ----- helper: compute twin primes up to N -------------------------------- */
static void compute_twin_primes(uint64_t N)
{
    uint64_t sz = N + 1;
    bool *prime = (bool*)malloc(sz);
    memset(prime, true, sz);
    if (sz > 0) prime[0] = false;
    if (sz > 1) prime[1] = false;

    for (uint64_t i = 2; i * i <= N; ++i)
        if (prime[i])
            for (uint64_t j = i * i; j <= N; j += i)
                prime[j] = false;

    size_t count = 0;
    for (uint64_t i = 2; i + 2 <= N; ++i)
        if (prime[i] && prime[i + 2]) ++count;

    twin_low  = (uint64_t*)realloc(twin_low,  count * sizeof(uint64_t));
    twin_high = (uint64_t*)realloc(twin_high, count * sizeof(uint64_t));
    twin_cnt  = count;

    size_t idx = 0;
    for (uint64_t i = 2; i + 2 <= N; ++i)
        if (prime[i] && prime[i + 2]) {
            twin_low[idx]  = i;
            twin_high[idx] = i + 2;
            ++idx;
        }

    free(prime);
}

/* ----- helper: build bitset of representable sums up to N ------------------ */
static void compute_representable(uint64_t N)
{
    rep_limit = N;
    rep = (bool*)realloc(rep, (N + 1) * sizeof(bool));
    memset(rep, 0, (N + 1) * sizeof(bool));

    for (size_t i = 0; i < twin_cnt; ++i) {
        uint64_t l = twin_low[i];
        if (l > N) break;
        for (size_t j = 0; j < twin_cnt; ++j) {
            uint64_t u = twin_high[j];
            uint64_t s = l + u;
            if (s > N) break;
            if ((s & 1ULL) == 0)
                rep[s] = true;
        }
    }
}

/* ----- helper: rebuild cache up to LIMIT --------------------------------- */
static void rebuild_cache(uint64_t limit)
{
    /* free previous cache */
    free(cache);
    cache   = NULL;
    cache_cap = 0;
    cache_sz  = 0;

    for (uint64_t e = 2; e <= limit; e += 2) {
        if (!rep[e]) {
            if (cache_sz >= cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_sz++] = (seq_t)e;
        }
    }
}

/* ----- helper: extend cache until it contains at least N terms ------------ */
static void fill_cache_up_to(idx_t n)
{
    if (cache_sz >= (size_t)n) return;

    /* initial limit chosen to cover many terms easily */
    uint64_t limit = 200000;

    while (cache_sz < (size_t)n) {
        compute_twin_primes(limit);
        compute_representable(limit);
        rebuild_cache(limit);

        if (cache_sz >= (size_t)n) break;

        limit *= 2;   /* increase limit and retry */
        if (limit > 20000000)   /* safety guard */
            break;
    }
}

/* ----- public sequence function ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; undefined for n=0 */
    fill_cache_up_to(n);
    return cache[n - 1];
}
