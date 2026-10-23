/*
 * A044353  Numbers n such that string 2,1 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains_21
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044353 – numbers n such that the decimal string "21" occurs in n
   but not in n‑1.  The code builds the sequence incrementally:
   a(1)=21, and for k>1 a(k) is the smallest integer >a(k-1) that has the
   required property.  A simple search is used, with results cached
   for fast random access.  All arithmetic is performed with the
   user‑supplied unsigned integer type seq_t; no floating point or
   external libraries are required. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/contains_21.h"

/* compute the n-th term of the sequence, 1‑based index */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;                /* undefined for offset < 1 */

    static seq_t *cache = NULL;          /* 1‑based cache: cache[1] = a(1) */
    static size_t cap = 0;               /* allocated array size          */
    static idx_t max_n = 0;              /* number of terms cached so far */
    static seq_t last_val = 0;           /* last computed term            */

    /* one‑time initialization */
    if (!cache) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failure – unlikely */
        cache[1] = (seq_t)21;            /* a(1) = 21 */
        max_n = 1;
        last_val = (seq_t)21;
    }

    /* ensure storage for requested index */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;       /* allocation failure – unlikely */
        cache = tmp;
        cap = new_cap;
    }

    /* generate subsequent terms until we have a(n) */
    while (max_n < n) {
        seq_t cand = last_val + (seq_t)1;
        for (;;) { /* search forward for next valid number */
            if (contains_21(cand) && !contains_21(cand - (seq_t)1)) {
                ++max_n;
                cache[max_n] = cand;
                last_val = cand;
                break;
            }
            ++cand;
        }
    }

    return cache[(size_t)n];
}
