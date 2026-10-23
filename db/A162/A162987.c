/*
 * A162987  Number of reduced words of length n in Coxeter group on 11
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A162987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(0)=1, a(1)=11, a(2)=110, a(3)=1100, a(4)=10945
 *   a(n) = 9*a(n-1)+9*a(n-2)+9*a(n-3)-45*a(n-4)   for n ≥ 5
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* cached values */
static size_t cache_len = 0;    /* number of values stored (>=5 after init) */

seq_t a(idx_t n)
{
    /* initialise the first five values when first called */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(5 * sizeof(seq_t));
        if (!cache) abort();

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)11;
        cache[2] = (seq_t)110;
        cache[3] = (seq_t)1100;
        /* a(4) = 9*(a3+a2+a1) - 45*a0 + 1 */
        seq_t sum = cache[3] + cache[2] + cache[1];
        cache[4] = 9 * sum - 45 * cache[0] + (seq_t)1;

        cache_len = 5;
    }

    /* return immediately if value is already cached */
    if (n < (idx_t)cache_len)
        return cache[n];

    /* enlarge the cache if needed */
    if ((idx_t)cache_len <= n) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_len = new_len;
    }

    /* compute missing terms up to n */
    for (size_t i = 5; i <= (size_t)n; ++i) {
        seq_t sum = cache[i - 1] + cache[i - 2] + cache[i - 3];
        cache[i] = 9 * sum - 45 * cache[i - 4];
    }

    return cache[n];
}
