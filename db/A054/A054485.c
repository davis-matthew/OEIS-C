/*
 * A054485  Expansion of (1+3*x)/(1-4*x+x^2).
 * https://oeis.org/A054485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - a(n-2), a(0) = 1, a(1) = 7
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for computed terms.  It grows as needed. */
static seq_t *cache      = NULL;   /* array of terms */
static size_t cache_size = 0;      /* number of cached terms */
static size_t cache_cap  = 0;      /* current capacity of the array */

/* Compute the n-th term of the OEIS sequence A054485. */
seq_t a(idx_t n)
{
    /* Initialise the cache on first call. */
    if (cache == NULL) {
        cache_cap = 2;                     /* start with space for two terms */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)                 /* allocation failed – return 0 */
            return 0;
        cache[0] = (seq_t)1;               /* a(0) */
        cache[1] = (seq_t)7;               /* a(1) */
        cache_size = 2;
    }

    /* Grow the cache until it contains a(n). */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        seq_t next = cache[i-1] * (seq_t)4; /* 4*a(i-1) */
        next = next - cache[i-2];          /* subtract a(i-2) */

        /* Resize the cache if necessary. */
        if (cache_size >= cache_cap) {
            cache_cap <<= 1;               /* double the capacity */
            seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            if (tmp == NULL)               /* allocation failed – abort */
                return 0;
            cache = tmp;
        }
        cache[cache_size++] = next;
    }

    return cache[(size_t)n];
}
