/*
 * A231413  Number of (n+1) X (1+1) 0..2 arrays with no element equal to a
 *          strict majority of its horizontal, diagonal and antidiagonal
 *          neighbors, with values 0..2 introduced in row major order.
 * https://oeis.org/A231413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-3) + 4*a(n-4) (n ≥ 5) Initial values:
 *            a(1)=9, a(2)=50, a(3)=285, a(4)=1617 Implements memoised
 *            computation; can be called in any order
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External integer types supplied by the build system */
seq_t a(idx_t n);

static seq_t *cache = NULL;   /* cache[i] holds a(i+1) */
static idx_t cache_size = 0;  /* number of terms stored in cache */

/* Return the n‑th term of A231413 (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* outside the OEIS offset */

    /* Ensure the cache has room for terms up to index n-1 */
    if (n > cache_size) {
        /* Reallocate array to size n (C++ needs explicit cast) */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – free existing cache and return 0 */
            free(cache);
            cache = NULL;
            cache_size = 0;
            return (seq_t)0;
        }
        cache = tmp;

        /* Compute missing terms, using 0‑based indexing */
        for (idx_t i = cache_size; i < n; ++i) {
            if (i == 0)
                cache[i] = (seq_t)9;    /* a(1) */
            else if (i == 1)
                cache[i] = (seq_t)50;   /* a(2) */
            else if (i == 2)
                cache[i] = (seq_t)285;  /* a(3) */
            else if (i == 3)
                cache[i] = (seq_t)1617; /* a(4) */
            else {
                /* Recurrence: a(n) = 6*a(n-1) - 11*a(n-3) + 4*a(n-4) */
                seq_t term = 6 * cache[i-1];
                term -= 11 * cache[i-3];
                term += 4 * cache[i-4];
                cache[i] = term;
            }
        }
        cache_size = n;
    }

    return cache[n-1];
}
