/*
 * A019309  Number of "bifix-free" words of length n over a four-letter
 *          alphabet.
 * https://oeis.org/A019309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1,4,12,48,180,720,… (OEIS A019309) * Recurrence: * a(0) =
 *            1, a(1) = 4 * a(2n+1) = 4 * a(2n) * a(2n) = 4 * a(2n-1) - a(n) *
 *            * The function memoises results in a simple dynamic array so
 *            that * calls in arbitrary order remain O(1) after a one‑time
 *            growth
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache      = NULL;   /* pointer to cached values          */
static size_t cache_cap  = 0;      /* allocated capacity (in elements)  */
static size_t cache_len  = 0;      /* number of elements stored         */

seq_t a(idx_t n)
{
    /* initialise with the two known seeds */
    if (cache_len == 0) {
        cache_cap = 4;                          /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;            /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache_len = 2;
    }

    /* trivial base cases */
    if (n == 0) return cache[0];
    if (n == 1) return cache[1];

    /* grow cache if necessary */
    if (n >= (idx_t)cache_len) {
        /* Resize the array to at least (n+1) elements. */
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 4;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until big enough */

        if (new_cap > cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;                /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute missing terms sequentially. */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            if (i & 1U) {               /* odd: i = 2k+1 */
                cache[i] = (seq_t)4 * cache[i - 1];
            } else {                    /* even: i = 2k */
                size_t k = i / 2;
                seq_t t1 = (seq_t)4 * cache[i - 1]; /* 4 * a(2k-1) */
                seq_t t2 = cache[k];                /* a(k)      */
                cache[i] = t1 - t2;                  /* 4*a(2k-1)-a(k) */
            }
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
