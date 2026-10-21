/*
 * A203147  (n-1)-st elementary symmetric function of {11, 12, 13, 14, ..., 10
 *          + n}.
 * https://oeis.org/A203147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{i=1}^{n-1}(10+i) + (10+n)*a(n-1) (with a(1)=1,
 *            the product for n=1 is empty = 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n) {
    if (n < 1)
        return (seq_t)0;          /* undefined for n<1, return 0 */

    /* Static cache: a(n) and product P(n)=∏_{i=1}^n(10+i) */
    static seq_t *cache_a = NULL;
    static seq_t *cache_P = NULL;
    static size_t cache_cap = 0;   /* allocated size (index 0 unused) */
    static idx_t  cache_max = 0;   /* highest n computed */

    /* Grow cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp_a = seq_realloc(cache_a, cache_cap, new_cap);
        seq_t *tmp_P = seq_realloc(cache_P, cache_cap, new_cap);
        if (!tmp_a || !tmp_P) abort();
        cache_a = tmp_a;  cache_P = tmp_P;
        cache_cap = new_cap;
    }

    /* Initialise if first call */
    if (cache_max == 0) {
        cache_a[1]  = (seq_t)1;   /* a(1) = 1 */
        cache_P[1]  = (seq_t)11;  /* P(1) = 11 */
        cache_max   = 1;
    }

    /* Compute a(i) for i = cache_max+1 .. n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t mult = (seq_t)i + (seq_t)10;              /* current element 10+i   */
        seq_t nextP = cache_P[i - 1] * mult;           /* P(i) = P(i-1)*(10+i)   */
        seq_t nextA = mult * cache_a[i - 1] + cache_P[i - 1]; /* a(i) = (10+i)*a(i-1)+P(i-1) */
        cache_P[i] = nextP;
        cache_a[i] = nextA;
    }
    cache_max = n;
    return cache_a[n];
}
