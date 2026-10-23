/*
 * A016146  Expansion of g.f. 1/((1-3*x)*(1-11*x)).
 * https://oeis.org/A016146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((11^(n+1) – 3^(n+1)) / 8); recurrence: a(0)=1, a(1)=14,
 *            a(n)=14*a(n-1) – 33*a(n-2) for n≥2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap  = 0;
    static idx_t  up_to  = 0;   /* largest index cached */

    /* initialise cache with first two values */
    if (cache == NULL) {
        const size_t init_cap = 2;
        cache = seq_alloc(init_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)14;
        cap   = init_cap;
        up_to = 1;
    }

    /* extend cache if required */
    if (n > up_to) {
        size_t needed = (size_t)(n + 1);
        if (needed > cap) {
            size_t new_cap = cap ? cap * 2 : 2;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = seq_realloc(cache, cap, new_cap);
            cache = tmp;
            cap   = new_cap;
        }

        /* compute missing terms using the recurrence */
        for (idx_t i = up_to + 1; i <= n; ++i) {
            cache[i] = (seq_t)14 * cache[i - 1] - (seq_t)33 * cache[i - 2];
        }
        up_to = n;
    }

    return cache[n];
}
