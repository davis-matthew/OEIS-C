/*
 * A174369  Numbers k such that 12k-5, 12k+1, and 12k+7 are prime.
 * https://oeis.org/A174369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A174369: a(n) is the integer k such that
   12k-5, 12k+1 and 12k+7 are all prime.  The sequence has no
   known closed form, so we generate terms by simple incremental
   search and cache the results. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* ---------- cache state ---------- */
static seq_t *cache     = NULL;   /* array of found k values          */
static size_t cache_cap = 0;      /* current cache capacity           */
static size_t cache_len = 0;      /* number of cached k values        */
static uint64_t last_k  = 0;      /* largest k that has been tested   */

/* ensure the cache can hold at least 'needed' elements */
static void ensure_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* generate sequence terms up to 'target' (1‑based index) */
static void generate_terms(idx_t target)
{
    if (cache_len >= target) return;  /* already computed */

    ensure_cache(target);             /* guarantee enough space */

    /* iterate k from last_k+1 upwards until we have enough terms */
    for (;; /* advance inside */) {
        ++last_k;
        seq_t n1 = (seq_t)12 * (seq_t)last_k - (seq_t)5;
        seq_t n2 = (seq_t)12 * (seq_t)last_k + (seq_t)1;
        seq_t n3 = (seq_t)12 * (seq_t)last_k + (seq_t)7;
        if (is_prime_big(n1) && is_prime_big(n2) && is_prime_big(n3)) {
            cache[cache_len++] = (seq_t)last_k;
            if (cache_len >= target) return;
        }
    }
}

/* required function: returns a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for n < 1, return 0 */
    generate_terms(n);
    return cache[n - 1];
}
