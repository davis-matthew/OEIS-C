/*
 * A097686  a(n+1) = if a(n) is even then a(n)/2 else (a(n) + number of
 *          earlier occurrences of a(n)), a(1) = 1.
 * https://oeis.org/A097686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A097686
   a(1) = 1
   For n ≥ 1:
       a(n+1) = a(n)/2                     if a(n) is even
                a(n) + (# of earlier occurrences of a(n))   if a(n) is odd
   The values of earlier occurrences are counted among all terms
   a(1), … , a(n).  This implementation caches computed terms and
   expands on demand; each new term is evaluated in O(n) time, so
   overall O(n²) work for reaching a given n (≤ ~10 000).  That
   bound comfortably meets the 2‑second limit. */

#include <stdlib.h>

/* null arguments (e.g. n==0) return 0 */
seq_t a(idx_t n)
{
    /* cache[0] is unused to allow 1‑based indexing */
    static seq_t *cache   = NULL;   /* array of computed terms */
    static size_t cache_cap = 0;    /* allocated size */
    static size_t cache_len = 0;    /* number of terms stored (>=0) */

    /* nothing requested */
    if (n == 0) return 0;

    /* initialise */
    if (cache_len == 0) {
        /* allocate a first, minimal block */
        cache_cap = 128;
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        if (!cache) return 0;          /* out of memory */
        cache[0] = 0;                  /* dummy */
        cache[1] = (seq_t)1;           /* a(1) */
        cache_len = 1;
    }

    /* grow cache until it contains term n */
    if (n > cache_len) {
        if (cache_cap <= n) {
            /* double until large enough */
            while (cache_cap <= n) cache_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, (cache_cap + 1) * sizeof(seq_t));
            if (!tmp) return 0;          /* out of memory */
            cache = tmp;
        }
        /* compute terms from cache_len+1 up to n */
        for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
            seq_t prev = cache[i-1];
            /* count earlier occurrences of prev */
            size_t earlier = 0;
            for (size_t j = 1; j < i-1; ++j)
                if (cache[j] == prev) ++earlier;
            seq_t next;
            if ((prev % (seq_t)2) == 0) {          /* even */
                next = prev / (seq_t)2;
            } else {                               /* odd */
                next = prev + (seq_t)earlier;
            }
            cache[i] = next;
        }
        cache_len = (size_t)n;
    }

    return cache[n];
}
