/*
 * A044314  Numbers n such that string 7,0 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains_70
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044314: Numbers n such that the base‑9 representation of n contains
   the substring “70” but the base‑9 representation of n ‑ 1 does not.
   Computed by brute‑force search with memoisation. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains_70.h"

/* Cache for the sequence values.  cache[i] holds a(i+1). */
static seq_t *cache = NULL;
static idx_t  cache_size = 0;   /* number of terms currently stored */
static size_t cache_cap  = 0;   /* capacity of `cache` */

/* Next integer to test for inclusion in the sequence. */
static seq_t next_candidate = 1;

/* Ensure that the cache contains at least the first `n` terms (1‑based). */
static void ensure_cache(idx_t n)
{
    if (n == 0)
        return;              /* no terms requested */
    if (cache_size >= n)
        return;              /* already enough terms */

    /* Initialise the cache if this is the first call. */
    if (cache_cap == 0) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(*cache));
    }

    /* Generate terms until we have at least `n` of them. */
    while (cache_size < n) {
        seq_t cand = next_candidate;

        if (contains_70(cand) && !contains_70(cand - 1)) {
            if (cache_size == cache_cap) {          /* grow array */
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(*cache));
                if (!tmp) abort();                  /* OOM: abort in this context */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = cand;
        }
        next_candidate = cand + 1;   /* proceed to next integer */
    }
}

/* Public sequence function. 1‑based index. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* undefined for offset 0, never called */
    ensure_cache(n);
    return cache[n - 1];
}
