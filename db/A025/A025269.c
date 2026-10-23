/*
 * A025269  a(n) = a(1)*a(n-1) + a(2)*a(n-2) + ...+ a(n-1)*a(1) for n >= 5,
 *          with initial values 2,1,1,1.
 * https://oeis.org/A025269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(1)*a(n-1)+a(2)*a(n-2)+...+a(n-1)*a(1) for n≥5, with
 *            initial values a(1)=2, a(2)=1, a(3)=1, a(4)=1
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  max_computed = 0;

    if (n == 0) return 0; /* undefined, but safe placeholder */

    /* Expand cache if necessary */
    if ((size_t)n > cap) {
        size_t new_cap   = (size_t)n;
        seq_t *new_cache = NULL;

#ifdef __cplusplus
        new_cache = new seq_t[new_cap];
#else
        new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
        if (!new_cache) return 0; /* allocation failure */

        /* Copy old values and initialise new ones */
        for (size_t i = 0; i < cap; ++i)
            new_cache[i] = cache[i];
        for (size_t i = cap; i < new_cap; ++i)
            new_cache[i] = 0;

        /* Free old cache */
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
        cache = new_cache;
        cap   = new_cap;
    }

    /* Compute new terms up to n if needed */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 4) {               /* initial values */
            if (i == 1) cache[0] = (seq_t)2;
            else        cache[i-1] = (seq_t)1;
        } else {                    /* convolution recurrence */
            seq_t sum = 0;
            for (idx_t j = 1; j < i; ++j)
                sum += cache[j-1] * cache[i-j-1];
            cache[i-1] = sum;
        }
    }

    max_computed = n;
    return cache[n-1];
}
