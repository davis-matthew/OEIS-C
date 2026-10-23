/*
 * A044423  Numbers n such that string 9,1 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has91, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A044423 by brute‑force search.
 * Numbers whose decimal representation contains the substring 91
 * but the predecessor does not.  For arbitrary precision seq_t
 * and 64‑bit idx_t the function fills a static cache.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_realloc.h"   /* provides seq_realloc */
#include "../utils/has91.h"

/* Static cache of found terms. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;  /* number of terms stored in cache */
static size_t cache_cap  = 0;  /* allocated capacity of cache    */
static seq_t next_candidate = 1;   /* next integer to test (always > last tested) */

/* Ensure that cache contains at least n terms. */
static void ensure_terms(idx_t n)
{
    for (; cache_len < n; ++cache_len) {
        /* Find the next number satisfying the condition. */
        seq_t m = next_candidate;
        while (true) {
            if (has91(m) && !has91(m - 1)) {
                /* Grow cache if necessary. */
                if (cache_len == cache_cap) {
                    size_t newcap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = seq_realloc(cache, cache_cap, newcap);
                    if (!tmp) { /* allocation failed; leave cache unchanged */
                        cache_len = cache_cap; /* avoid infinite loop in caller */
                        return;
                    }
                    cache    = tmp;
                    cache_cap = newcap;
                }
                cache[cache_len] = m;
                next_candidate   = m + 1;
                break;
            }
            ++m;
        }
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined for n < 1 */

    ensure_terms(n);
    return cache[n - 1];
}
