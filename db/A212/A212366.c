/*
 * A212366  Number of Dyck n-paths all of whose ascents and descents have
 *          lengths equal to 1 (mod 7).
 * https://oeis.org/A212366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1 a(n)=a(n-1)+Σ_{k=1}^{n-7} a(k)*a(n-7-k) for n>0 For 0≤n≤7
 *            the sum is empty so a(n)=1
 * Range:     u8 n <= 19, u16 n <= 33, u32 n <= 59, u64 n <= 110,
 *            u128 n <= 209, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] stores a(i) */
    static seq_t *cache = NULL;
    static size_t sz   = 0;   /* number of stored values */
    static size_t cap  = 0;   /* allocated capacity (>= sz) */

    /* initialise on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL)
            return 0;          /* out of memory */
        cache[0] = 1;
        sz = 1;
        cap = 1;
    }

    /* expand cache if needed one time before computing missing values */
    if (n >= sz) {
        size_t needed = (size_t)n + 1;
        if (needed > cap) {
            size_t new_cap = (cap > 0) ? cap : 1;
            while (new_cap < needed)
                new_cap <<= 1;                 /* double until enough */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL)
                return 0;                      /* allocation failed */
            cache = tmp;
            cap = new_cap;
        }

        /* compute all missing values up to n */
        for (idx_t i = sz; i <= n; ++i) {
            if (i <= 7) {
                cache[i] = 1;          /* a(0)…a(7)=1 */
            } else {
                seq_t sum = cache[i - 1];     /* start with a(i-1) */
                for (idx_t k = 1; k <= i - 7; ++k) {
                    sum += cache[k] * cache[i - 7 - k];
                }
                cache[i] = sum;
            }
        }
        sz = n + 1;
    }

    return cache[n];
}
