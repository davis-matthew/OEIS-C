/*
 * A238315  An oscillating sequence: a(n) = n^2 + 2^(n-1) - 2*a(n-1), n>0,
 *          with a(1) = 1.
 * https://oeis.org/A238315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + 2^(n-1) - 2*a(n-1), a(1) = 1
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    static idx_t last = 0;          /* largest n for which cache is filled */
    static seq_t *cache = NULL;    /* 1‑based cache stored as 0‑based array */
    static size_t cap   = 0;        /* allocated size of cache */

    /* Initial allocation if this is the first call */
    if (!cache) {
        cap   = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Grow cache if necessary to fit up to index n */
    if (n > cap) {
        size_t new_cap = cap * 2;
        if (n > new_cap)
            new_cap = n * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failure -> abort */
            return (seq_t)0;
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise the very first term if the cache is empty */
    if (last == 0) {
        cache[0] = (seq_t)1;   /* a(1) = 1 */
        last    = 1;
    }

    /* Compute missing terms up to n */
    for (idx_t k = last + 1; k <= n; ++k) {
        seq_t sq   = (seq_t)k * (seq_t)k;            /* k^2 */
        seq_t p2   = seq_pow((seq_t)2, k - 1);       /* 2^(k-1) */
        seq_t prev = cache[k - 2];                   /* a(k-1) */
        cache[k - 1] = sq + p2 - ((seq_t)2 * prev); /* recurrence */
    }
    last = n;   /* cache now contains up to a(n) */

    return cache[n - 1];
}
