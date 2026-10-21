/*
 * A214846  Square array T, read by antidiagonals: T(n,k) = 0 if n-k >= 6 or
 *          if k-n >= 6, T(k,0) = T(0,k) = 1 if 0 <= k <= 5, T(n,k) = T(n-1,k)
 *          + T(n,k-1).
 * https://oeis.org/A214846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry read by antidiagonals from the square array T
 *            defined by T(n,k) = 0 if |n-k| >= 6 T(n,0) = T(0,k) = 1 if 0 <=
 *            k <= 5 T(n,k) = T(n-1,k) + T(n,k-1) otherwise a(0) is the first
 *            element of that reading
 * Range:     u8 n <= 69, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Persistent cache of computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Arrays for the current and the previous antidiagonal. */
    static seq_t *prev = NULL;
    static seq_t *curr = NULL;
    static size_t prev_len = 0;     /* length of prev = previous diagonal size   */
    static size_t curr_cap = 0;     /* capacity of curr                         */
    static idx_t last_d = 0;        /* last diagonal index that has been fully processed */

    /* Initialise on first call. */
    if (cache == NULL) {
        cache_cap  = 16;
        cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                     /* fatal error */

        /* diagonal d = 0 */
        prev_len = 1;
        prev     = (seq_t *)malloc(prev_len * sizeof(seq_t));
        if (!prev) return 0;
        prev[0] = 1;
        cache[0] = prev[0];
        cache_len = 1;
        last_d = 0;
    }

    /* Return already computed element, if possible. */
    if (n < cache_len) return cache[n];

    /* Generate further antidiagonals until we reach index n. */
    for ( ; cache_len <= n; ++last_d ) {
        idx_t d   = last_d + 1;                 /* current diagonal to compute */
        size_t diag_size = (size_t)(d + 1);     /* number of entries on this diagonal */

        /* Ensure buffer for the current diagonal. */
        if (curr_cap < diag_size) {
            curr_cap = diag_size;
            curr = (seq_t *)realloc(curr, curr_cap * sizeof(seq_t));
            if (!curr) return 0;
        }

        /* Compute all elements of diagonal d. */
        for (size_t i = 0; i <= d; ++i) {
            size_t j = (size_t)(d - i);
            seq_t val;

            /* Check the band condition. */
            int64_t diff = (int64_t)i - (int64_t)j;
            if (diff < 0) diff = -diff;
            if (diff > 5) {            /* outside the band -> zero */
                val = 0;
            } else if (i == 0) {       /* first row inside the band */
                val = 1;
            } else if (j == 0) {       /* first column inside the band */
                val = 1;
            } else {                   /* interior cell */
                /* Both neighbors belong to the previous diagonal. */
                seq_t a1 = prev[i-1];
                seq_t a2 = prev[i];
                val = a1 + a2;
            }

            curr[i] = val;

            /* Append to the global cache. */
            if (cache_len == cache_cap) {
                cache_cap = cache_cap * 2 + 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) return 0;
            }
            cache[cache_len++] = val;
        }

        /* Prepare for next iteration: the new diagonal becomes the previous one. */
        seq_t *tmp      = prev;
        prev            = curr;
        curr            = tmp;
        prev_len        = diag_size;
    }

    return cache[n];
}
