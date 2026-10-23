/*
 * A162022  Smallest prime factor of n-th odd composite integers A071904.
 * https://oeis.org/A162022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime factor of the n-th odd composite integer
 *            (A071904). a(1) is the smallest prime factor of 9, a(2) of 15,
 *            etc
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- helper: smallest prime factor for an odd number ---------- */
static seq_t spf_of(uint64_t n)
{
    /* n is odd; returns the smallest prime divisor of n.
       For a prime n the function returns n itself. */
    uint64_t d;
    for (d = 3; d <= n / d; d += 2) {
        if (n % d == 0) return (seq_t)d;
    }
    return (seq_t)n;          /* n is prime */
}

/* ---------- static cache for computed terms ---------- */
static seq_t  *cache     = NULL;   /* array of a(n) values (0‑based)       */
static size_t  cache_sz  = 0;      /* number of values stored            */
static size_t  cache_cap = 0;      /* current capacity of cache array    */
static uint64_t next_odd = 9;      /* next odd integer to examine        */

/* Ensure that the cache contains terms up to index n (1‑based). */
static void ensure_cache(size_t n)
{
    if (n <= cache_sz) return;            /* already present */

    /* enlarge the array if needed */
    while (cache_cap < n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* out of memory */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* produce new entries until we reach n */
    while (cache_sz < n) {
        uint64_t val = next_odd;
        next_odd += 2;                    /* move to next odd number */

        seq_t spf = spf_of(val);
        if (spf == val) {                 /* val is prime, skip */
            continue;
        }

        cache[cache_sz++] = spf;          /* store a(n) for current val */
    }
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset 1, safe guard */
    ensure_cache((size_t)n);
    return cache[(size_t)n - 1];          /* cache is 0‑based */
}
