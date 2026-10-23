/*
 * A182432  Recurrence a(n)*a(n-2) = a(n-1)*(a(n-1) + 3) with a(0) = 1, a(1) =
 *          4.
 * https://oeis.org/A182432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 4; a(n) = 8*a(n-1) - a(n-2) - 3 for n ≥ 2
 *            (equivalent to a(n)*a(n-2) = a(n-1)*(a(n-1)+3))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /*  Memoization buffer.  */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;   /* total allocated elements     */
    static size_t cache_len  = 0;   /* number of elements actually used */

    /* Handle the two known base cases.  */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    /* Initialise the cache on the first call.  */
    if (cache == NULL) {
        /* Allocate enough space for at least the first two terms and the
           requested index, rounding up to a power of two. */
        size_t initial_cap = (n >= 2 ? n + 1 : 2);
        size_t cap = 1;
        while (cap < initial_cap) cap <<= 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure */
        cache_cap = cap;
        cache_len = 2;                        /* we now know a(0) and a(1) */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
    }

    /* If the requested index is beyond the current allocation, grow the
       buffer.  */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;   /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;             /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n.  */
    if (cache_len <= n) {
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            cache[i] = (seq_t)8 * cache[i - 1] - cache[i - 2] - (seq_t)3;
        }
        cache_len = n + 1;
    }

    return cache[(size_t)n];
}
