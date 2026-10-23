/*
 * A144788  a(n+1) = a(n)^3 - a(n) + 1 with a(1) = 3.
 * https://oeis.org/A144788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n+1) = a(n)^3 - a(n) + 1, a(1) = 3
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1u)
        return (seq_t)0;

    /* static cache: cache[i] holds a(i+1) */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* number of terms currently stored   */
    static size_t cache_cap = 0;  /* allocated capacity of the array    */

    /* initialise cache with a(1) */
    if (!cache)
    {
        cache_cap = 1;
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache)
            return (seq_t)0; /* allocation failure – return 0 */
        cache[0] = (seq_t)3; /* a(1) */
    }

    /* increase capacity if necessary */
    if (cache_cap < (size_t)n)
    {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)n);
        if (!tmp)
            return (seq_t)0; /* allocation failure – return 0 */
        cache = tmp;
        cache_cap = (size_t)n;
    }

    /* compute any missing terms up to a(n) */
    while (cache_len < (size_t)n)
    {
        seq_t prev = cache[cache_len - 1];
        seq_t term = prev * prev * prev - prev + (seq_t)1; /* a(k+1) */
        cache[cache_len] = term;
        ++cache_len;
    }

    return cache[n - 1];
}
