/*
 * A089749  a(n) = smallest m > a(n-1) such that concatenation nm is a prime.
 * https://oeis.org/A089749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest m > a(n‑1) such that concatenation nm is a prime
 *            (computed by searching forward for the next prime for each n)
 * Range:     u8 n <= 3, u16 n <= 1542, u32 n <= 6086, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   concat_parts, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/isPrimeGeneric.h"   /* bool isPrimeGeneric(seq_t n) */
#include "../utils/concat_parts.h"     /* seq_t concat_parts(idx_t a, idx_t b) */

/* cache holds the sequence values for indices >= 1.
   cache[0] == a(1), cache[1] == a(2), ... */
static seq_t *cache = NULL;
static size_t cache_cap = 0;  /* number of allocated slots */

/* Ensure that cache has at least n slots, zero‑initialising any new ones.
   Returns false on allocation failure. */
static bool ensure_cache(idx_t n)
{
    if ((size_t)n <= cache_cap) return true;
    size_t new_cap = (size_t)n;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) return false;
    /* zero the newly allocated part   */
    memset(new_cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n == 0 */

    if (!ensure_cache(n)) return 0; /* allocation failure */

    /* initialise a(1) if it hasn't been set yet */
    if (cache[0] == 0) cache[0] = 1;

    /* Compute terms that are still missing. */
    for (size_t idx = 1; idx < (size_t)n; ++idx)   /* idx+1 == current n */
    {
        if (cache[idx] != 0) continue;            /* already known */

        seq_t prev = cache[idx - 1];              /* a(idx) = a(n-1) */
        seq_t m    = prev + 1;                    /* start search */

        for (;; ++m) {
            seq_t candidate = concat_parts((idx_t)(idx + 1), (idx_t)m);
            if (isPrimeGeneric(candidate)) {
                cache[idx] = m;                  /* a(idx+1) = m */
                break;
            }
        }
    }

    return cache[n - 1];
}
