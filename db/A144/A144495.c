/*
 * A144495  Row 2 of array in A144502.
 * https://oeis.org/A144495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((4+3*n)*a(n-1) - (n+3)*(n-1)*a(n-2) +
 *            (n-1)*(n-2)*a(n-3)) / 2 with a(0) = 2, a(1) = 7, a(2) = 30
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;       /* memoisation buffer */
    static size_t cache_cap = 0;          /* allocated size of cache    */
    static size_t cache_len = 0;          /* number of values stored    */

    /* Ensure there is space for indices up to n (at least 3 for the base case) */
    size_t need = (size_t)n + 1;
    if (need < 3) need = 3;
    if (need > cache_cap)
    {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failed – unlikely */
        cache      = tmp;
        cache_cap  = need;
    }

    /* Initialise the first three terms once */
    if (cache_len == 0)
    {
        cache[0] = 2;
        cache[1] = 7;
        cache[2] = 30;
        cache_len = 3;
    }

    /* Compute any missing values up to n */
    if (cache_len <= n)
    {
        for (idx_t i = cache_len; i <= n; ++i)
        {
            /* All arithmetic is done in seq_t to avoid overflow */
            seq_t term1 = ((seq_t)(4 + 3 * i)) * cache[i - 1];
            seq_t term2 = ((seq_t)(i + 3)) * ((seq_t)(i - 1)) * cache[i - 2];
            seq_t term3 = ((seq_t)(i - 1)) * ((seq_t)(i - 2)) * cache[i - 3];
            seq_t num   = term1 - term2 + term3;
            cache[i]    = num / ((seq_t)2);
        }
        cache_len = n + 1;
    }

    return cache[n];
}
