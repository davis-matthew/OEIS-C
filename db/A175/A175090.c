/*
 * A175090  Composites c with result 0 under iterations of {r mod (max prime p
 *          <= r)} starting at r = c.
 * https://oeis.org/A175090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 112, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A175090: composites c such that repeated application of
   r = r mod (max prime ≤ r) eventually reaches 0. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/prev_prime.h"

/* Cache of computed terms */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;
static idx_t next_to_test = 4;

/* Test whether a composite c eventually leads to 0 under the described process */
static bool leads_to_zero(idx_t c)
{
    idx_t r = c;
    while (r > 1) {
        if (is_prime(r))
            return true;                      /* hits a prime, next step will be 0 */
        idx_t p = prev_prime(r);              /* largest prime < r */
        if (p == 0)                           /* should never happen for r ≥ 4 */
            return false;
        r = r % p;
        if (r == 0)
            return true;                      /* reached 0 directly */
    }
    return false;                              /* failed (reached 1) */
}

/* Ensure the cache contains at least n terms */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n)
        return;

    /* Allocate or grow cache */
    if (cache_len == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_len == cache_cap) {
        cache_cap *= 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Search for more terms */
    while (cache_len < n) {
        /* Skip primes */
        if (!is_prime(next_to_test)) {
            if (leads_to_zero(next_to_test)) {
                /* store term */
                if (cache_len == cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)next_to_test;
            }
        }
        next_to_test++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)               /* not part of OEIS, return 0 as safe fallback */
        return 0;
    ensure_cache(n);
    return cache[n - 1];
}
