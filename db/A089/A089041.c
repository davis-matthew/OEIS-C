/*
 * A089041  Inverse binomial transform of squares of factorial numbers.
 * https://oeis.org/A089041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2*a(n-1) + n*(n-1)*a(n-2) + (-1)^n with a(0) = 1, a(1)
 *            = 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Persistent cache for computed values */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Compute a( n ) */
seq_t a(idx_t n)
{
    /* Initialise cache with first two terms */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
    }

    /* Compute up to requested index if needed */
    if (n >= cache_len) {
        /* Grow cache to hold indices up to n */
        idx_t new_len = n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        /* Fill new entries using recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* i >= 2 because cache_len at least 2 from init */
            seq_t term = (seq_t)i * (seq_t)i * cache[i - 1];
            term += (seq_t)i * (seq_t)(i - 1) * cache[i - 2];
            if (i & 1)
                term -= (seq_t)1;   /* (-1)^i = -1 for odd i */
            else
                term += (seq_t)1;   /* (+1) for even i */
            cache[i] = term;
        }
        cache_len = new_len;
    }

    return cache[n];
}
