/*
 * A082579  Expansion of e.g.f.: exp( x/(1-x)^2 ).
 * https://oeis.org/A082579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=5. For n ≥ 3 let m = n-3 a(n) =
 *            (3*m+7)*a(n-1) - (m+2)*(3*m+2)*a(n-2) + (m+2)*(m+1)*m*a(n-3)
 *            This recurrence is derived from the OEIS relation a(n+3) -
 *            (3*n+7)*a(n+2) + (n+2)*(3*n+2)*a(n+1) - (n+2)*(n+1)*n*a(n) = 0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of results */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with first three terms */
    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;         /* out of memory, return 0 as sentinel */
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 5;
    }

    /* if requested term is already cached */
    if (n < cache_size)
        return cache[n];

    /* extend cache up to n */
    idx_t old_size = cache_size;
    cache_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
    if (!new_cache) return 0;  /* allocation failed */
    cache = new_cache;

    /* compute missing terms using the recurrence */
    for (idx_t i = old_size; i <= n; ++i) {
        /* i corresponds to n in the recurrence; m = i-3 */
        idx_t m = i - 3;

        /* cast indices to seq_t for arithmetic */
        seq_t sm = (seq_t)m;
        seq_t coeff1 = (seq_t)(3 * m + 7);
        seq_t coeff2 = (seq_t)((m + 2) * (3 * m + 2));
        seq_t coeff3 = (seq_t)((m + 2) * (m + 1) * m);

        /* a[i] = coeff1 * a[i-1] - coeff2 * a[i-2] + coeff3 * a[i-3] */
        seq_t term1 = coeff1 * cache[i - 1];
        seq_t term2 = coeff2 * cache[i - 2];
        seq_t term3 = coeff3 * cache[i - 3];

        cache[i] = term1 - term2 + term3;
    }

    return cache[n];
}
