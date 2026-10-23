/*
 * A193941  G.f.: (1+x^3)/(1-x-x^6).
 * https://oeis.org/A193941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=1, a(3)=2, a(4)=2, a(5)=2; for n≥6:
 *            a(n)=a(n-1)+a(n-6) (generating function (1+x³)/(1−x−x⁶))
 * Range:     u8 n <= 23, u16 n <= 45, u32 n <= 89, u64 n <= 178,
 *            u128 n <= 355, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: stores a(k) for k = 0 … last_computed. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;     /* allocated entries */
    static idx_t last_computed = (idx_t)-1;  /* -1 indicates no values yet */

    /* Allocate or enlarge the cache to accommodate index n. */
    if (cache == NULL || cache_size <= n) {
        idx_t new_size = n + 1;
        /* realloc is safe because seq_t may be arbitrary-precision; the
           resulting objects are value-initialized when subsequently assigned. */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – return 0 (this case should not occur). */
            return (seq_t)0;
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* Initial values for indices 0…5. */
    static const seq_t base[6] = { (seq_t)1, (seq_t)1, (seq_t)1,
                                   (seq_t)2, (seq_t)2, (seq_t)2 };

    /* Ensure base values are present if we have not computed them yet. */
    if (last_computed < 5) {
        idx_t limit = 5;
        if (n < limit) limit = (idx_t)n;
        for (idx_t i = 0; i <= limit; ++i) {
            cache[i] = base[i];
        }
        if (last_computed < limit)
            last_computed = limit;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i >= 6) {
            cache[i] = cache[i - 1] + cache[i - 6];
        } else {
            cache[i] = base[i];
        }
    }
    if (last_computed < n)
        last_computed = n;

    return cache[n];
}
