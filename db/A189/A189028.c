/*
 * A189028  Zero-one sequence based on the sequence (5n-4):
 *          a(A016861(k))=a(k); a(A047203(k))=1-a(k); a(1)=0.
 * https://oeis.org/A189028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Self‑generating zero‑one sequence A189028.
   a(1) = 0.
   For n > 1:
         if n ≡ 1 (mod 5)        →  a(n) = a((n+4)/5)
         else                    →  a(n) = 1 – a(n – ⌊(n+4)/5⌋)   */

/* Include the standard allocation header. */
#include <stdlib.h>

/* Cached values of the sequence; they are indexed from 1. */
static seq_t *cache      = NULL;
static idx_t cache_len   = 0;    /* largest index that has been computed. */

seq_t a(idx_t n)
{
    /* Guard against n == 0 – not used by the sequence but harmless. */
    if (n == 0)
        return (seq_t)0;

    /* Allocate or grow the cache so that indices up to n are present. */
    if (n > cache_len) {
        /* New size needed. */
        size_t new_size = (size_t)(n + 1);      /* indices 0 … n */
        if (!cache) {
            /* Allocate fresh cache. */
            cache = (seq_t*)malloc(new_size * sizeof(seq_t));
        } else {
            /* Extend existing cache. */
            cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        }
        if (!cache)   /* allocation failed – abort safely. */
            abort();

        /* Initialize cache[1] if it hasn't been already. */
        if (cache_len < 1)
            cache[1] = (seq_t)0;

        /* Compute new entries. */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i % 5 == 1) {
                /* i = 5k – 4 → k = (i+4)/5. */
                idx_t k = (i + 4) / 5;
                cache[i] = cache[k];
            } else {
                /* i is not of the form 5k – 4.
                   Count of numbers of the form 5k‑4 up to i is
                   floor((i+4)/5).  The index of i in the v‑sequence
                   is thus i – floor((i+4)/5). */
                idx_t k = i - ((i + 4) / 5);
                cache[i] = 1 - cache[k];
            }
        }
        cache_len = n;
    }

    /* Return the already computed value. */
    return cache[n];
}
