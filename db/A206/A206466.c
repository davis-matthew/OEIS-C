/*
 * A206466  Number of (n+1) X 3 0..2 arrays with every 2 X 2 subblock having
 *          zero permanent.
 * https://oeis.org/A206466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 176*a(n-3) + 256*a(n-4) with initial values:
 *            a(1) = 121 a(2) = 1411 a(3) = 11025 a(4) = 106891 (OEIS A206466)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* The function returns the n-th term of the sequence.
   It uses memoization; each term is computed only once. */
seq_t a(idx_t n)
{
    /* Handle out of bounds: for n < 1 we simply return 0. */
    if (n < 1)
        return (seq_t)0;

    /* Static cache of computed values.  Index 0 is unused; cache[i] holds a(i). */
    static seq_t *cache = NULL;      /* pointer to allocated array */
    static idx_t  cache_len = 0;     /* number of terms stored (max index) */
    static idx_t  cache_cap = 0;     /* allocated capacity */

    /* Helper to grow cache to at least cap elements. */
    if (cache_cap < n + 1) {         /* need room for indices up to n */
        idx_t new_cap = (cache_cap > 0) ? cache_cap * 2 : 16;
        while (new_cap < n + 1)
            new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!new_cache) {
            /* Allocation failed: return 0 as a safety net. */
            return (seq_t)0;
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Fill base cases up to n if necessary. */
    if (cache_len < 4) {
        /* Ensure initial values are stored. */
        if (cache_len < 1) {
            cache[1] = (seq_t)121;   /* a(1) */
        }
        if (cache_len < 2) {
            cache[2] = (seq_t)1411;  /* a(2) */
        }
        if (cache_len < 3) {
            cache[3] = (seq_t)11025; /* a(3) */
        }
        if (cache_len < 4) {
            cache[4] = (seq_t)106891;/* a(4) */
        }
        cache_len = 4;
    }

    /* Compute in order from cache_len+1 up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* Recurrence: a(i) = 11*a(i-1) - 176*a(i-3) + 256*a(i-4) */
        seq_t term1 = (seq_t)11 * cache[i - 1];
        seq_t term2 = (seq_t)176 * cache[i - 3];
        seq_t term3 = (seq_t)256 * cache[i - 4];
        cache[i] = term1 - term2 + term3;
    }
    cache_len = n;   /* max index now computed */

    return cache[n];
}
