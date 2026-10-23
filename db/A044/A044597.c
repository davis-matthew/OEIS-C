/*
 * A044597  Numbers n such that string 3,5 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   contains35octal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that the string "35" occurs in the base‑8
 * representation of n but not in the base‑8 representation of n+1.
 *
 * Computed by incremental search with memoisation. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/contains35octal.h"

/* Memoised cache of the sequence values. */
static seq_t *cache = NULL;      /* array of found terms */
static size_t cache_len = 0;      /* number of terms stored */
static size_t cache_cap = 0;      /* allocated capacity */
static seq_t next_candidate = 1;  /* next integer to check */

/* Ensure the cache has room for at least one more element. */
static void ensure_cache_capacity(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) {
            /* In the unlikely event of memory exhaustion we abort. */
            abort();
        }
        cache = new_ptr;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* 1‑based index: return the n‑th term of the sequence. */
    if (n == 0) {
        return 0;  /* behaviour for n < 1 is irrelevant */
    }

    /* Return cached value if already computed. */
    if ((size_t)n <= cache_len) {
        return cache[n - 1];
    }

    /* Extend the cache until we have computed the n‑th term. */
    while (cache_len < (size_t)n) {
        /* Make sure there is room for the new term. */
        ensure_cache_capacity();

        /* Search for the next integer that satisfies the condition. */
        while (true) {
            /* Check the present candidate. */
            if (contains35octal(next_candidate) &&
                !contains35octal(next_candidate + 1)) {
                cache[cache_len] = next_candidate;
                cache_len++;
                next_candidate++;  /* start after the found value */
                break;
            }
            next_candidate++;  /* try the next integer */
        }
    }

    return cache[n - 1];
}
