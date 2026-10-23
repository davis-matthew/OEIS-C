/*
 * A175451  a(n)>a(n-1), a(n) = smallest prime such that a(n)+a(n-1) is
 *          multiple of m, a(1)=2, m=21.
 * https://oeis.org/A175451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A175451: a(1)=2; for n>1, a(n) is the smallest prime > a(n‑1)
   such that (a(n)+a(n‑1)) % 21 == 0 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;        /* array of cached terms (index 0 unused) */
static size_t cache_len = 0;       /* highest index currently stored */
static size_t cache_cap = 0;       /* capacity of the allocated array */

/* Ensure the cache has room for at least 'need' slots (0 .. need-1). */
static void ensure_capacity(size_t need)
{
    if (cache_cap >= need) return;
    if (cache == NULL) {
        cache = (seq_t *)malloc(need * sizeof(seq_t));
    } else {
        cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
    }
    /* If allocation failed, terminate – the build will not proceed. */
    if (!cache) exit(1);
    cache_cap = need;
}

/* Find the next prime greater than 'prev' that satisfies
   (prev + prime) % 21 == 0. */
static seq_t next_prime_after(seq_t prev)
{
    /* Determine the desired residue for the next prime. */
    seq_t prev_mod = prev % 21;
    seq_t target = (21 - prev_mod) % 21;
    if (target == 0) target = 21;           /* never happens for primes */

    /* Start candidate as the first number > prev in the correct residue class. */
    seq_t candidate = prev + 1;
    seq_t cand_mod = candidate % 21;
    seq_t diff;
    if (cand_mod <= target)
        diff = target - cand_mod;
    else
        diff = target + 21 - cand_mod;
    candidate += diff;

    /* Search forward in steps of 21. */
    while (true) {
        if (is_prime_big(candidate)) return candidate;
        candidate += 21;
    }
}

/* Make sure the sequence is computed up to index 'n'. */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;
    size_t idx = (size_t)n;
    if (idx <= cache_len) return;

    ensure_capacity(idx + 1);          /* allocate up to index 'idx' */

    if (cache_len == 0) {              /* initialise the first term */
        cache[1] = 2;
        cache_len = 1;
    }

    for (size_t i = cache_len + 1; i <= idx; ++i) {
        cache[i] = next_prime_after(cache[i - 1]);
    }
    cache_len = idx;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n=0, return neutral value */
    ensure_cache(n);
    return cache[(size_t)n];
}
