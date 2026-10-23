/*
 * A043739  Numbers whose number of runs in the base-2 representation is
 *          congruent to 0 mod 7.
 * https://oeis.org/A043739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   runs_mod7_is0, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043739: numbers whose number of runs in the binary
 * representation is 0 mod 7.
 * The function returns the n‑th such number (a(1)=85, a(2)=149, …).
 */

#include <stdbool.h>            /* for bool */
#include <stddef.h>             /* for size_t */
#include "../utils/seq_alloc.h"   /* seq_alloc()   */
#include "../utils/seq_realloc.h" /* seq_realloc() */
#include "../utils/runs_mod7_is0.h"

/* Static cache of found terms */
static seq_t *cache = NULL;     /* array of terms */
static size_t cache_len = 0;    /* current number of stored terms */
static size_t cache_cap = 0;    /* allocated capacity */
static seq_t last_found = 0;    /* last term that satisfied the condition */

/* Ensure that the cache contains at least n terms. */
static void grow_cache(idx_t n)
{
    size_t needed = (size_t)n;
    while (cache_len < needed) {
        /* Find next number that satisfies the condition. */
        seq_t candidate = last_found + 1;

        while (!runs_mod7_is0(candidate))
            candidate++;

        /* Store the candidate. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap == 0 ? 256 : cache_cap * 2;
            seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
            if (!tmp) abort();          /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = candidate;
        last_found = candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defined only for n >= 1 */
    grow_cache(n);
    return cache[n - 1];
}
