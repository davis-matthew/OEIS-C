/*
 * A090729  a(n) = 21a(n-1) - a(n-2), starting with a(0) = 2 and a(1) = 21.
 * https://oeis.org/A090729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 21*a(n-1) - a(n-2), a(0)=2, a(1)=21
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first two elements */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)21;

    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;          /* pointer to array of terms */
    static size_t cache_len = 0;         /* number of allocated slots */
    static size_t max_cached = 1;        /* highest index that has been computed */

    /* Lazy allocation of the first two elements */
    if (cache == NULL)
    {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failure; caller can handle */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)21;
    }

    /* Grow the buffer if needed */
    if ((size_t)n >= cache_len)
    {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure */
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute any missing terms up to n */
    if ((size_t)n > max_cached)
    {
        for (size_t i = max_cached + 1; i <= (size_t)n; ++i)
        {
            /* Use unsigned __int128 to avoid overflow during the multiplication
               before truncating to seq_t. This works for builtin integral types
               and has no effect on an arbitrary‑precision seq_t implementation. */
            seq_t product = (seq_t)((unsigned __int128)21 * cache[i - 1]);
            cache[i] = product - cache[i - 2];
        }
        max_cached = (size_t)n;
    }

    return cache[(size_t)n];
}
