/*
 * A037108  Trajectory of 3 under map n->21*n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A037108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 17, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Trajectory of 3 under map n → 21*n+1 if n is odd, n→n/2 if n is even */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t   sz   = 0;   /* number of elements stored          */
    static size_t  cap  = 0;   /* allocated capacity                  */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;       /* allocation failure      */
        cache[0] = (seq_t)3;                      /* a(0)                     */
        sz = 1;
        cap = 1;
    }

    /* if requested index already computed, return it */
    if (n < sz) return cache[n];

    /* compute terms up to n, storing them */
    for (idx_t i = sz; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t val  = (prev & 1)          /* odd?        */
                      ? (prev * (seq_t)21 + (seq_t)1)
                      : (prev >> 1);                 /* even: n/2  */

        /* expand cache if necessary */
        if ((size_t)i >= cap) {
            size_t new_cap = cap ? cap * 2 : 1;
            while (new_cap <= (size_t)i) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0;          /* allocation fail */
            cache = tmp;
            cap   = new_cap;
        }
        cache[i] = val;
    }

    sz = n + 1;
    return cache[n];
}
