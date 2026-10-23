/*
 * A044784  Numbers n such that string 7,1 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains_71
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044784: Numbers n such that the decimal string "71"
   occurs in n but not in n+1. This implementation searches
   sequentially and memoises results; no closed‑form formula is used. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/contains_71.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static seq_t cur_candidate = 1;

    if (n == 0)  /* offset is 1; return 0 for n==0 as a safe default. */
        return (seq_t)0;

    if (!cache) {
        cache_cap = 128;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;
    }

    while (cache_size < n) {
        if (contains_71(cur_candidate) && !contains_71(cur_candidate + (seq_t)1)) {
            if (cache_size == cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) { free(cache); cache = NULL; return (seq_t)0; }
                cache = tmp;
            }
            cache[cache_size++] = cur_candidate;
        }
        cur_candidate++;
    }
    return cache[n - 1];
}
