/*
 * A050084  a(n) = floor(a(n-1)/3) if this is not among 0, a(1), ..., a(n-2);
 *          otherwise a(n) = 5*a(n-1).
 * https://oeis.org/A050084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 262, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   contains
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* SEQUENCE: A050084
   a(1) = 1
   for n > 1:
      let k = floor(a(n-1)/3)
      if k == 0 or k has appeared among {a(1), …, a(n-2)} then
         a(n) = 5·a(n-1)
      else
         a(n) = k
*/

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains.h"   /* bool contains(const seq_t *, idx_t, seq_t) */

static seq_t *cache = NULL;        /* 0‑based array: cache[i] = a(i+1) */
static idx_t cache_cap = 0;        /* allocated capacity */
static idx_t cache_len = 0;        /* number of terms stored */

/* Ensure cache has space for at least 'need' elements. */
static void grow_cache(idx_t need)
{
    if (cache_cap >= need)
        return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failed – not expected */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 – return 0 */

    /* Initialise cache with the first term on first call. */
    if (cache_len == 0) {
        grow_cache(1);
        cache[0] = (seq_t)1;   /* a(1) */
        cache_len = 1;
    }

    /* If n has already been computed, return the cached value. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Compute terms from the current length up to n. */
    grow_cache(n);
    for (idx_t idx = cache_len; idx < n; ++idx) {
        seq_t prev = cache[idx - 1];
        seq_t candidate = prev / 3;

        bool duplicate = (candidate == (seq_t)0) ||
                         contains(cache, idx, candidate);
        cache[idx] = duplicate ? prev * (seq_t)5 : candidate;
    }
    cache_len = n;
    return cache[n - 1];
}
