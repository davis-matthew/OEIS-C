/*
 * A164849  Diagonal sum of (100^n,1) Pascal triangle.
 * https://oeis.org/A164849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A164849 Diagonal sum of (100^n,1) Pascal triangle
   Recurrence: a(n) = a(n-1) + 101*a(n-2) – 100*a(n-3) – 100*a(n-4)
   Initial terms (offset 1): a(1)=1, a(2)=1, a(3)=101, a(4)=102
   Indexing: a(1) is the first term. */

#include <stddef.h>
#include <stdlib.h>

/* Cache previous terms: 0‑based index corresponds to a(n) with n=index+1 */
static seq_t *cache = NULL;     /* pointer to cached terms */
static size_t cache_len = 0;    /* number of terms cached */

/* Return the nth term (n >= 1) */
seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1, but guard against 0 for safety */
    if (n == 0)
        return 0;

    /* Initialise cache with the first four known values */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;          /* allocation error (unlikely) */
        cache[0] = 1;   /* a(1) */
        cache[1] = 1;   /* a(2) */
        cache[2] = 101; /* a(3) */
        cache[3] = 102; /* a(4) */
    }

    /* Expand the cache if n exceeds current size */
    if ((idx_t)cache_len < n) {
        size_t new_len = (size_t)n;
        seq_t *tmp =
            (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation error */
        cache = tmp;

        /* Compute terms from the first missing index up to n-1 */
        /* 0‑based indices i correspond to a(i+1) */
        for (idx_t i = cache_len; i < n; ++i) {
            seq_t term = cache[i-1]          /* a(i)   */
                       + 101 * cache[i-2]     /* 101*a(i-1) */
                       - 100 * cache[i-3]     /* 100*a(i-2) */
                       - 100 * cache[i-4];    /* 100*a(i-3) */
            cache[i] = term;
        }
        cache_len = new_len;
    }

    return cache[n-1];      /* return the requested term */
}
