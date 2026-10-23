/*
 * A115090  a(n) = A115074(n) - A117183(n).
 * https://oeis.org/A115090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 411, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A115090
   Formula: a(n) = A046665(A013929(n))
   - A013929(n): nth nonsquarefree integer
   - A046665(m): largest prime divisor of m minus smallest prime divisor of m
   The implementation precomputes a small smallest-prime-factor table,
   generates the required nonsquarefree integers on demand,
   and stores the resulting differences for quick repeat access.
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Type definitions are supplied by the build (seq_t, idx_t). */

#define SIEVE_LIMIT 200000   /* sufficiently large for n <= 10000 */

static uint32_t *spf = NULL;
static size_t spf_size = 0;

/* ------------------------------------------------------------ */
/* Build a smallest‑prime‑factor (SPF) table up to LIMIT.       */
/* ------------------------------------------------------------ */
static void build_spf(size_t limit)
{
    if (spf_size >= limit)
        return;                          /* already built up to this size */

    free(spf);
    spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!spf)
        exit(1);                          /* out of memory */
    spf_size = limit;

    for (size_t i = 0; i <= limit; ++i)
        spf[i] = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                 /* i is prime */
            spf[i] = (uint32_t)i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0)
                        spf[j] = (uint32_t)i;
            }
        }
    }
}

/* ------------------------------------------------------------ */
/* Test whether x is nonsquarefree and compute diff = maxpf - minpf */
/* ------------------------------------------------------------ */
static bool nonsq_and_diff(uint64_t x, seq_t *diff)
{
    uint64_t temp = x;
    uint64_t smallest = 0;
    uint64_t largest = 0;
    bool nonsquarefree = false;

    while (temp > 1) {
        uint32_t p = spf[temp];
        uint64_t cnt = 0;
        while (temp % p == 0) {
            temp /= p;
            ++cnt;
        }
        if (cnt >= 2)
            nonsquarefree = true;
        if (smallest == 0)
            smallest = p;
        if (p > largest)
            largest = p;
    }

    *diff = (seq_t)(largest - smallest);
    return nonsquarefree;
}

/* ------------------------------------------------------------ */
/* Internal cache for memoisation.                              */
/* ------------------------------------------------------------ */
static seq_t  *cache  = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;     /* number of values stored */

static uint64_t next_candidate = 1;

/* Grow cache capacity when needed. */
static void grow_cache(void)
{
    if (cache_len < cache_cap)
        return;
    cache_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    if (!cache)
        exit(1);
}

/* Ensure that values up to n are computed. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len)
        return;

    if (!cache)
        grow_cache();  /* initialise cache capacity */

    while (cache_len < (size_t)n) {
        ++next_candidate;
        seq_t diff;
        if (nonsq_and_diff(next_candidate, &diff)) {
            grow_cache();
            cache[cache_len++] = diff;
        }
    }
}

/* ------------------------------------------------------------ */
/* Main entry point.                                            */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Build SPF table once. */
    static bool built = false;
    if (!built) {
        build_spf(SIEVE_LIMIT);
        built = true;
    }

    ensure_cache(n);
    return cache[n - 1];   /* 1‑based indexing */
}
