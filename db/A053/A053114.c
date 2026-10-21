/*
 * A053114  a(n) = ((8*n+9)(!^8))/9, related to A045755 ((8*n+1)(!^8) octo- or
 *          8-factorials).
 * https://oeis.org/A053114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) * (8*n + 9), a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cached values */
static seq_t *cache = NULL;
static idx_t  last   = (idx_t)-1;  /* largest n computed so far */
static size_t cache_cap = 0;       /* allocated size */

/* Return the n‑th term of OEIS A053114 */
seq_t a(idx_t n)
{
    /* Ensure the cache array is available and large enough. */
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();          /* allocation failure */
        cache[0] = (seq_t)1;
        last = 0;
    } else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();            /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing values up to n. */
    if (n > last) {
        for (idx_t k = last + 1; k <= n; ++k) {
            /* factor = 8*k + 9, computed in seq_t to avoid idx_t overflow */
            seq_t factor = (seq_t)8 * (seq_t)k + (seq_t)9;
            cache[k] = cache[k - 1] * factor;
        }
        last = n;
    }

    return cache[n];
}
