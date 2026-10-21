/*
 * A096405  Numbers having an equal number of zeros in their binary and
 *          ternary representations.
 * https://oeis.org/A096405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 3095, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   zeros_in_bin, zeros_in_tern
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers having an equal number of zeros in their binary and ternary representations (OEIS A096405).
   The function builds the sequence by scanning integers and caching the
   found terms.  It works for any seq_t (built‑in or arbitrary‑precision)
   as long as seq_t is assignable from a scalar of type seq_t.  The cache
   is grown with realloc, with an explicit cast to the seq_t* type. */
#include <stdlib.h>
#include "../utils/zeros_in_bin.h"
#include "../utils/zeros_in_tern.h"

/* Cache for the seen terms.  Static so it survives across calls. */
static seq_t *cache   = NULL;  /* pointer to the terms already found */
static idx_t cache_len = 0;    /* number of terms stored */
static idx_t cache_cap = 0;    /* allocated capacity */
static idx_t last_checked = 0; /* next integer to test */

/* Return the n-th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    /* For robustness handle n==0, though OEIS offset is 1. */
    if (n == 0)
        return (seq_t)0;

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n) {
        idx_t x = last_checked;
        while (1) {
            if (zeros_in_bin(x) == zeros_in_tern(x)) {
                /* Store the term in the cache. */
                if (cache_len == cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 16;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    /* realloc may return NULL on failure; the harness assumes sufficient memory. */
                }
                cache[cache_len++] = (seq_t)x;
                /* Next search will start after this number. */
                last_checked = x + 1;
                break;
            }
            x++;
        }
    }
    return cache[n - 1];
}
