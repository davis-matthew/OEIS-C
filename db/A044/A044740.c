/*
 * A044740  Numbers n such that string 2,7 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains_27
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n whose decimal representation contains the substring
   "27" but (n+1) does not. This implementation builds the sequence
   by brute‑force search, caching found terms. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/contains_27.h"

/* Cached values of the sequence; grown on demand. */
static seq_t *cache   = NULL;
static size_t cache_size = 0;   /* number of valid entries */
static size_t cache_cap   = 0;   /* allocated capacity */
static seq_t next_candidate = (seq_t)1;   /* next integer to test */

/* Return true iff x contains "27" but (x+1) does not. */
static bool qualifies(seq_t x) {
    return contains_27(x) && !contains_27(x + (seq_t)1);
}

/* Return the nth term of the sequence (offset 1). */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined, but not used */

    while (cache_size < (size_t)n) {
        /* grow the cache if necessary */
        if (cache_size == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();      /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }
        if (qualifies(next_candidate))
            cache[cache_size++] = next_candidate;
        next_candidate = next_candidate + (seq_t)1;
    }
    return cache[n - 1];
}
