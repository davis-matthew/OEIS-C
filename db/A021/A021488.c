/*
 * A021488  Decimal expansion of 1/484.
 * https://oeis.org/A021488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = a(n‑1) - a(n‑11) + a(n‑12) (n > 13) * Initial terms: *
 *            a(0)=0, a(1)=0, a(2)=2, a(3)=0, a(4)=6, a(5)=6, a(6)=1, *
 *            a(7)=1, a(8)=5, a(9)=7, a(10)=0, a(11)=2, a(12)=4, a(13)=7
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache of computed terms */
static seq_t *cache = NULL;     /* cache[0 .. cap-1] are valid */
static idx_t  cap   = 0;        /* number of cached terms */

static const seq_t init_terms[14] = {
    0, 0, 2, 0, 6, 6, 1, 1, 5, 7, 0, 2, 4, 7
};

seq_t a(idx_t n)
{
    /* Determine the required cache size */
    idx_t required = n + 1;
    if (required < 14)          /* at least the seed terms */
        required = 14;
    if (required > cap) {
        /* Resizing the cache */
        seq_t *new_cache = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (!new_cache)        /* allocation failure */
            return 0;
        cache = new_cache;

        /* Initialize missing terms */
        for (idx_t i = cap; i < required; ++i) {
            if (i < 14) {                    /* seed values */
                cache[i] = init_terms[i];
            } else {
                /* Use signed arithmetic to avoid unsigned under‑flow */
                long long t = (long long)cache[i-1]
                                   - (long long)cache[i-11]
                                   + (long long)cache[i-12];
                cache[i] = (seq_t)t;
            }
        }
        cap = required;
    }
    return cache[n];
}
