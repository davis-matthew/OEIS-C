/*
 * A044615  Numbers n such that string 5,7 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has57oct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS sequence A044615.
   A(n) is the n-th positive integer whose base‑8 representation
   contains the substring "57" while the base‑8 representation of
   n+1 does not. This is implemented by enumerating integers
   and caching results for efficiency. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has57oct.h"

static seq_t *cache = NULL;          // cached sequence values
static size_t cache_len = 0;         // number of terms cached
static size_t cache_cap = 0;         // capacity of the cache array
static seq_t next_candidate = 1;     // next integer to test

// Extend the cache until it contains at least n terms.
static void extend_cache(idx_t n)
{
    // Use size_t for array indices; cast n safely.
    while ((idx_t)cache_len < n) {
        if (has57oct(next_candidate)) {
            seq_t nxt = next_candidate + 1;
            if (!has57oct(nxt)) {
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = next_candidate;
            }
        }
        next_candidate++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          // undefined for offset 1; return 0
    extend_cache(n);
    return cache[n - 1];
}
