/*
 * A211213  n-alternating permutations of length 3n.
 * https://oeis.org/A211213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(3*n,n)*binomial(2*n,n) - 2*binomial(3*n,n) + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper function binomial(idx_t n, idx_t k) */
/* Cached values are stored in a static array for fast repeated calls */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cached_len = 0;

    /* Grow the cache to accommodate index n (zero‑based). */
    if (n >= cached_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        /* Initialize new entries to zero. */
        for (size_t i = cached_len; i < new_len; ++i) {
            tmp[i] = 0;
        }
        cache = tmp;
        cached_len = new_len;
    }

    /* Return memoized value if present. */
    if (cache[(size_t)n] != 0)
        return cache[(size_t)n];

    /* Compute the term using the closed form. */
    seq_t c3n_n = binomial(3 * n, n);
    seq_t c2n_n = binomial(2 * n, n);
    seq_t prod   = c3n_n * c2n_n;
    seq_t ans    = prod - (2 * c3n_n) + 1;

    cache[(size_t)n] = ans;
    return ans;
}
