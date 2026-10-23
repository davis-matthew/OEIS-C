/*
 * A028119  Expansion of 1/((1-4x)(1-5x)(1-7x)(1-12x)).
 * https://oeis.org/A028119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 28*a(n-1) - 275*a(n-2) + 1136*a(n-3) - 1680*a(n-4), with
 *            a(0)=1, a(1)=28, a(2)=509, a(3)=7688
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* for malloc/realloc in C mode */

seq_t a(idx_t n)
{
    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* number of values already computed */
    static size_t cache_cap  = 0;      /* allocated size */

    /* Initialise cache on first call. */
    if (cache == NULL) {
        cache_cap = 4;
#ifdef __cplusplus
        cache = new seq_t[cache_cap];
#else
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure – unlikely in tests */
#endif
    }

    /* Grow cache if needed to contain n. */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= n + 1) new_cap = n + 1;

#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_cap];
        for (size_t i = 0; i < cache_cap; ++i) tmp[i] = cache[i];
        delete [] cache;
#else
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
#endif
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    if (n >= cache_size) {
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 1;
            } else if (i == 1) {
                cache[1] = 28;
            } else if (i == 2) {
                cache[2] = 509;
            } else if (i == 3) {
                cache[3] = 7688;
            } else {
                seq_t term = 28 * cache[i - 1];
                term -= 275 * cache[i - 2];
                term += 1136 * cache[i - 3];
                term -= 1680 * cache[i - 4];
                cache[i] = term;
            }
        }
        cache_size = n + 1;
    }

    return cache[n];
}
