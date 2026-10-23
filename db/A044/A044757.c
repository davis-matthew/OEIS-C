/*
 * A044757  Numbers n such that string 4,4 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n whose decimal representation contains
   the substring "44" but whose successor n+1 does not.  The
   function returns the n‑th term (a(1) = 44).  It builds a
   static cache of found terms so that calls in any order
   remain fast (O(total terms found)).  The routine uses the
   utility function seq_to_str to examine decimal digits.  */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_to_str.h"

/* Static cache for terms already found */
static seq_t *cache      = NULL;   /* array of terms             */
static size_t cache_cap  = 0;      /* allocated capacity         */
static size_t cache_size = 0;      /* number of valid entries    */
static seq_t last_scanned = 0;     /* last integer examined      */

/* Return true if decimal representation of x contains
   consecutive digits '4''4'.  */
static bool has_44(seq_t x)
{
    /* Enough space for 128‑bit numbers (≤39 digits) and
       a generous safety margin for arbitrary‑precision. */
    char buf[2000];
    size_t len;
    seq_to_str(x, buf, &len);

    for (size_t i = 0; i + 1 < len; ++i)
        if (buf[i] == '4' && buf[i + 1] == '4')
            return true;
    return false;
}

/* Return the n‑th element of the sequence (n ≥ 1).  */
seq_t a(idx_t n)
{
    /* Return from cache if we already have the term. */
    if (cache_size >= (size_t)n)
        return cache[n - 1];

    /* Generate terms until we reach the requested index. */
    seq_t cand = last_scanned + 1;              /* start scanning */
    for (;; cand += 1) {
        if (cand == 0)                      /* skip possible overflow wrap */
            continue;
        if (has_44(cand) && !has_44(cand + 1)) {
            /* Grow the cache if necessary. */
            if (cache_size == cache_cap) {
                cache_cap = (cache_cap == 0 ? 64 : cache_cap * 2);
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = cand;    /* store found term */
            if (cache_size >= (size_t)n)  /* enough terms? */
                break;
        }
    }
    last_scanned = cand;                     /* remember last examined number */

    return cache[n - 1];
}
