/*
 * A092451  Numbers that either contain the digit 2 or are divisible by 2.
 * https://oeis.org/A092451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 165, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   contains2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n‑th non‑negative integer that is even or contains the digit 2. */
/* The function lazily generates the sequence and memoises it in a static array. */

#include <stdlib.h>
#include "../utils/contains2.h"

/* Cache to hold the sequence terms that have been generated so far. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;      /* number of elements currently stored */
static idx_t cache_cap = 0;      /* allocated capacity (in elements) */
static idx_t next_candidate = 1; /* next integer to examine (after 0) */

/* External function required by the build system. */
seq_t a(idx_t n)
{
    /* Initialise the cache on first call. */
    if (cache == NULL) {
        /* Start with a small capacity and grow it as needed. */
        cache_cap = 8;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);          /* out‑of‑memory: abort */
        cache[0] = (seq_t)0;          /* a(0) = 0 */
        cache_len = 1;
    }

    /* Generate terms until we have at least n+1 of them. */
    while (cache_len <= n) {
        seq_t val = (seq_t)next_candidate;      /* current candidate */
        if ((val % (seq_t)2 == (seq_t)0) || contains2(val)) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) exit(1);          /* out‑of‑memory: abort */
                cache = tmp;
            }
            cache[cache_len++] = val;
        }
        next_candidate++;                           /* try next integer */
    }

    return cache[(size_t)n];
}
