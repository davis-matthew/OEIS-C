/*
 * A044481  Numbers n such that string 4,3 occurs in the base 5 representation
 *          of n but not of n+1.
 * https://oeis.org/A044481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   has_43
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A044481: numbers n such that the base‑5 representation of n
   contains the substring "43" but the base‑5 representation of n+1 does not. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_43.h"

/* Cache for the generated terms */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static seq_t last_checked = (seq_t)0;

/* Extend the cache up to the given count (1‑based). */
static void generate_until(size_t needed)
{
    const seq_t one = (seq_t)1;
    seq_t candidate = last_checked + one;

    while (cache_len < needed) {
        if (has_43(candidate) && !has_43(candidate + one)) {
            /* Ensure capacity */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *new_ptr = (seq_t *)realloc(cache,
                                                  new_cap * sizeof(seq_t));
                if (!new_ptr) abort();   /* allocation failure */
                cache = new_ptr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = candidate;
        }
        candidate += one;
    }
    last_checked = candidate - one;
}

/* The required function. */
seq_t a(idx_t n)
{
    /* OEIS offset is 1. Return 0 for n==0 to keep behaviour defined. */
    if (n == 0) return (seq_t)0;

    size_t wanted = (size_t)n;          /* convert 1‑based index */
    while (cache_len < wanted)
        generate_until(wanted);

    return cache[wanted - 1];
}
