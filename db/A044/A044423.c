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
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
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
#include <stdlib.h>

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Return true if v contains the decimal substring 91. */
static bool has91(seq_t v)
{
    if (v < 10)
        return false;          /* need at least two digits */

    seq_t prev = v % 10;
    v /= 10;
    while (v > 0) {
        seq_t cur = v % 10;
        if (prev == 1 && cur == 9)   /* ...9,1... found (right‑to‑left) */
            return true;
        prev = cur;
        v /= 10;
    }
    return false;
}

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
