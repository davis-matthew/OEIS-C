/*
 * A044442  Numbers n such that string 0,0 occurs in the base 4 representation
 *          of n but not of n+1.
 * https://oeis.org/A044442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   has_00
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044442: numbers n such that the base‑4 representation of n
   contains the pair "00" whereas the base‑4 representation of n+1 does
   not.  The function builds the sequence on demand by brute force
   search.  It is memoised so arbitrary call order is supported. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_00.h"

/* static state for memoisation */
static seq_t *cache = NULL;          /* array of found terms          */
static idx_t  cached_len = 0;        /* number of terms stored        */
static size_t cache_cap  = 0;        /* allocated capacity            */
static idx_t  next_candidate = 1;    /* next integer to test          */

/* public function: returns the n‑th term, 1‑based index */
seq_t a(idx_t n)
{
    if (n == 0)          /* sequence is defined only for n≥1 */
        return 0;

    /* generate terms until we have n of them */
    while (cached_len < n) {
        seq_t val = (seq_t)next_candidate;

        if (has_00(val) && !has_00(val + 1)) {
            /* grow cache if necessary */
            if (cache_cap == 0) {
                cache_cap = 16;
                cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            } else if ((size_t)cached_len == cache_cap) {
                cache_cap <<= 1; /* double capacity */
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cached_len++] = val;
        }
        next_candidate++;
    }

    return cache[n - 1];
}
