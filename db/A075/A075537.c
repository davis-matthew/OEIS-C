/*
 * A075537  a(1)=1, a(2)=2, then use "merge and minus":
 *          a(n)=merge(a(n-2),a(n-1))-a(n-2)-a(n-1).
 * https://oeis.org/A075537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=1, a(2)=2; for n≥3, a(n)=merge(a(n-2),a(n-1))-a(n-2)-a(n-1)
 *            where merge(x,y) concatenates the decimal representations of x
 *            and y
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 10,
 *            big any n
 * Helpers:   merge_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/merge_seq.h"

/* -------------------------------------------------------------
   Cached evaluation of a(n).  The cache holds the already
   computed sequence values in a 0‑based array:
       cache[0] = a(1), cache[1] = a(2), …
   The function is safe for non‑increasing calls to a(n).
   ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* behaviour for n < 1 is irrelevant */

    static seq_t *cache = NULL;         /* array of computed values      */
    static uint64_t cached_terms = 0;   /* number of terms currently in cache */
    static uint64_t cache_cap = 0;      /* allocated capacity of cache   */

    /* Initialise cache with the first two values if needed. */
    if (cached_terms == 0) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;            /* a(1) */
        cache[1] = (seq_t)2;            /* a(2) */
        cached_terms = 2;
    }

    /* Extend the cache to include term a(n) if it is not yet present. */
    if ((uint64_t)n > cached_terms) {
        uint64_t old_terms = cached_terms;   /* terms already computed */

        /* Resize the backing array if the requested index exceeds it. */
        if ((uint64_t)n > cache_cap) {
            uint64_t new_cap = cache_cap ? cache_cap : 1;
            while (new_cap < (uint64_t)n) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute the missing terms one by one.  For i = old_terms+1 … n:
           a(i) = merge(a(i-2), a(i-1)) - a(i-2) - a(i-1).         */
        for (uint64_t i = old_terms + 1; i <= (uint64_t)n; ++i) {
            seq_t prev2 = cache[i - 3];   /* a(i-2) */
            seq_t prev1 = cache[i - 2];   /* a(i-1) */
            seq_t merged = merge_seq(prev2, prev1);
            cache[i - 1] = merged - prev2 - prev1;  /* store a(i) */
        }
        cached_terms = n;
    }

    return cache[n - 1];
}
