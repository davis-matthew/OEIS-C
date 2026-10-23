/*
 * A127680  a(0) = 1; a(n+1) = Sum_{k=0..n} a(n-k)*a(floor(k/2)).
 * https://oeis.org/A127680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1; a(n+1)=∑_{k=0}^{n} a(n-k)·a(floor(k/2))
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 61, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    /* initialise cache with a(0)=1 */
    if (cache == NULL) {
        cap  = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = 1;
        len = 1;
    }

    /* expand cache up to required index n */
    for (idx_t cur = len; cur <= n; ++cur) {
        idx_t m = cur - 1;          /* we need a(cur) from a(m) */
        idx_t half = m / 2;
        seq_t sum = 0;

        for (idx_t v = 0; v <= half; ++v) {
            const seq_t a_v = cache[v];

            idx_t idx1 = m - 2 * v;           /* index of a(m-2v)       */
            seq_t term = a_v * cache[idx1];   /* a(v) * a(m-2v)         */

            if (idx1 > 0)                     /* add a(v) * a(m-2v-1)  */
                term += a_v * cache[idx1 - 1];

            sum += term;
        }

        /* grow cache if necessary */
        if (len == cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);    /* unlikely, keep it simple */
            cache = tmp;
            cap = new_cap;
        }
        cache[len++] = sum;
    }

    return cache[n];
}
