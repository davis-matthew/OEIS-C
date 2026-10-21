/*
 * A087247  Squarefree deficient nonprime numbers.
 * https://oeis.org/A087247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   squarefree_and_sigma
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A087247 (square‑free deficient non‑prime numbers).
   The function searches integers in increasing order, keeps those that are
   square‑free, have sigma(n) < 2*n (deficient), and are not prime.  It
   caches results so that any order of calls is supported. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/squarefree_and_sigma.h"

/* Function to test primality is not required: we detect primes via sigma. */

/* Internal cache of computed terms. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;   /* number of terms stored            */
static size_t cache_cap  = 0;   /* allocated capacity of the cache    */
static uint64_t max_checked = 0;/* the largest integer examined so far */

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_cache(size_t need) {
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();               /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Public function as required by the interface. */
seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;  /* offset is 1 */

    /* Extend cache until it contains the nth term. */
    if (n > cache_len) {
        for (uint64_t m = max_checked + 1; cache_len < (size_t)n; ++m) {
            max_checked = m;

            seq_t sigma;
            bool sqfree = squarefree_and_sigma(m, &sigma);

            if (!sqfree)                               /* skip non‑sf      */
                continue;

            /* Detect primes: sigma(n) == n + 1  */
            if (sigma == (seq_t)m + 1)
                continue;                              /* skip primes      */

            /* deficient test */
            seq_t two_m = (seq_t)m * 2;
            if (sigma < two_m) {
                ensure_cache(cache_len + 1);
                cache[cache_len++] = (seq_t)m;
            }
        }
    }

    return cache[(size_t)n - 1];
}
