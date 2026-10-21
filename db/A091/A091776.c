/*
 * A091776  a(1)= 1, a(n+1) = a(n)*(a(n)+1)*(a(n)+2)*...*(a(n)+n-1)/n.
 * https://oeis.org/A091776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; * a(n+1) = a(n) * (a(n)+1) * ... * (a(n)+n-1) / n (n ≥
 *            1)
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, abort */

/* Cache for computed terms. */
static seq_t *cache   = NULL;   /* cache[i] holds a(i+1) */
static size_t cache_len = 0;    /* number of elements in cache */

/* Return the n‑th term of A091776 (a(1) is the first term). */
seq_t a(idx_t n)
{
    if (n == 0)                       /* undefined, but safe */
        return (seq_t)0;

    size_t needed = (size_t)n;

    if (needed > cache_len) {
        /* Grow the cache to the required size. */
        seq_t *new_cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!new_cache)
            abort();                  /* unlikely but guarantees defined behaviour */

        /* Copy previously computed values. */
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];

        free(cache);
        cache = new_cache;

        /* Remember old length before we overwrite it. */
        size_t old_len = cache_len;
        cache_len = needed;          /* final length after filling */

        /* Initialise a(1) if this is the first allocation. */
        if (old_len == 0) {
            cache[0] = (seq_t)1;
            old_len = 1;              /* a(1) is already computed */
        }

        /* Compute the remaining terms up to a(needed). */
        for (size_t i = old_len; i < needed; ++i) {
            idx_t cur_n = (idx_t)(i + 1);   /* current index (n ≥ 2) */
            seq_t prev      = cache[i - 1]; /* a(cur_n-1) */
            seq_t prod      = prev;         /* start with a(n-1) */

            /* Multiply by (a(n-1)+1) … (a(n-1)+n-2). */
            for (idx_t t = 1; t < cur_n - 1; ++t)
                prod *= (prev + (seq_t)t);

            /* Divide by (cur_n-1) == n-1 of the recurrence. */
            prod /= (seq_t)(cur_n - 1);

            cache[i] = prod;            /* store a(cur_n) */
        }
    }

    return cache[n - 1];
}
