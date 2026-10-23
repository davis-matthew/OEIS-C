/*
 * A140298  a(0)=1; a(3n+1) = a(3n)+1, a(3n+2) = a(3n+1) + a(3n) (=3*A000244),
 *          a(3n+3) = a(3n+2) + a(3n) (=A003462(n+2)).
 * https://oeis.org/A140298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(3n+1)=a(3n)+1; a(3n+2)=a(3n+1)+a(3n);
 *            a(3n+3)=a(3n+2)+a(3n);
 * Range:     u8 n <= 14, u16 n <= 29, u32 n <= 59, u64 n <= 121,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0; /* number of elements already computed */
static size_t cache_cap = 0; /* allocated capacity */

/* Compute the sequence value a(n) with memoization. */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;           /* indices are non‑negative */

    /* Initialize cache on first call. */
    if (cache == NULL) {
        cache_cap = 64u;              /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)                 /* out of memory */
            exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;         /* a(0) = 1 */
        cache_len = 1;
    }

    /* Grow the cache if necessary. */
    if (idx >= cache_len) {
        if (idx >= cache_cap) {
            size_t new_cap = cache_cap;
            while (new_cap <= idx)
                new_cap <<= 1;           /* double until large enough */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                exit(EXIT_FAILURE);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute values from cache_len to idx inclusively. */
        for (size_t i = cache_len; i <= idx; ++i) {
            if (i % 3 == 1) {           /* a(3n+1) = a(3n)+1 */
                cache[i] = cache[i - 1] + (seq_t)1;
            } else if (i % 3 == 2) {    /* a(3n+2) = a(3n+1)+a(3n) */
                cache[i] = cache[i - 1] + cache[i - 2];
            } else {                    /* i % 3 == 0, a(3n+3) = a(3n+2)+a(3n) */
                cache[i] = cache[i - 1] + cache[i - 3];
            }
        }
        cache_len = idx + 1;           /* next uncomputed index */
    }

    return cache[idx];
}
