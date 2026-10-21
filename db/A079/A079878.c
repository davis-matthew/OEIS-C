/*
 * A079878  a(1)=1, then a(n)=2*a(n-1) if 2*a(n-1)<=n, a(n)=2*a(n-1)-n
 *          otherwise.
 * https://oeis.org/A079878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>1: a(n)=2*a(n-1) if 2*a(n-1)<=n else 2*a(n-1)-n
 * Range:     u8 n <= 154, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the OEIS A079878 sequence.
 * The recurrence requires the previous value; the function keeps
 * a static cache so that arbitrary order of calls is supported
 * and repetitions are fast.
 */
seq_t a(idx_t n)
{
    /* Handle indices below the offset in a harmless way. */
    if (n < 1) return 0;

    /* Static cache for already computed terms. */
    static seq_t *cache = NULL;      /* array indexed from 1 .. max_n */
    static idx_t cache_cap = 0;      /* allocated size (>= max_n+1)    */
    static idx_t max_n = 0;          /* highest index computed so far  */

    /* Ensure capacity for n.  Allocate or grow the cache as needed. */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;       /* allocate enough for indices 0 .. n */
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (cache == NULL) return 0; /* allocation failure -> fallback */
        } else {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return 0;   /* allocation failure -> fallback */
            cache = tmp;
        }
        cache_cap = new_cap;
    }

    /* Initialise from the start if we have never computed anything. */
    if (max_n == 0) {
        cache[1] = (seq_t)1;
        max_n = 1;
    }

    /* Compute terms up to n if necessary. */
    if (n > max_n) {
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t doubled = prev + prev;          /* 2 * prev          */
            /* Compare using seq_t to avoid overflow of idx_t. */
            if (doubled <= (seq_t)i) {
                cache[i] = doubled;
            } else {
                cache[i] = doubled - (seq_t)i;
            }
        }
        max_n = n;
    }

    return cache[n];
}
