/*
 * A022870  a(n) = [ a(n-1)/a(1) ] + [ a(n-2)/a(2) ] + ... + [ a(1)/a(n-1) ],
 *          for n >= 3.
 * https://oeis.org/A022870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [a(n-1)/a(1)] + [a(n-2)/a(2)] + … + [a(1)/a(n-1)], n≥3;
 *            initial values: a(1)=a(2)=a(3)=2
 * Range:     u8 n <= 19, u16 n <= 35, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;       /* array with computed a(k), 0‑based   */
    static idx_t cache_size = 0;      /* number of elements currently allocated   */
    static idx_t max_computed = 0;    /* largest n for which a(n) is stored       */

    if (n < 1) return 0; /* undefined, but guard against zero input */

    /* enlarge the cache if it cannot hold a(n) */
    if (cache_size < (size_t)n) {
        idx_t new_cap = n > 32 ? n * 2 : 64;
        seq_t *tmp;
#ifdef __cplusplus
        tmp = new seq_t[new_cap];
        if (cache) {
            for (idx_t i = 0; i < cache_size; ++i) tmp[i] = cache[i];
            delete [] cache;
        }
#else
        tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
#endif
        cache = tmp;
        cache_size = new_cap;
    }

    /* initialise the first three terms if not already done */
    if (max_computed < 1) {
        cache[0] = 2;
        max_computed = 1;
    }
    if (max_computed < 2) {
        cache[1] = 2;
        max_computed = 2;
    }
    if (max_computed < 3) {
        cache[2] = 2;
        max_computed = 3;
    }

    /* if we already computed up to n, just return */
    if (n <= max_computed)
        return cache[n - 1];

    /* compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k < i; ++k) {
            sum += cache[i - k - 1] / cache[k - 1];
        }
        cache[i - 1] = sum;
    }
    max_computed = n;

    return cache[n - 1];
}
