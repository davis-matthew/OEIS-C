/*
 * A135294  a(n) = 3*a(n-1)+n if a(n-1) is not divisible by 2, or a(n) =
 *          a(n-1)/2 otherwise.
 * https://oeis.org/A135294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = 3*a(n-1)+(n-1) if a(n-1) is odd, a(n) =
 *            a(n-1)/2 if a(n-1) is even. ( OEIS offset 1 )
 * Range:     u8 n <= 49, u16 n <= 125, u32 n <= 439, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for any request below the offset; behaviour is
       unspecified for n == 0 but this guard keeps the function
       well‑defined. */
    if (n == 0) return (seq_t)0;

    /* Static cache storing already computed terms. */
    static seq_t *cache = NULL;
    static idx_t  computed = 0;   /* number of terms stored          */
    static size_t capacity = 0;   /* allocated size in terms          */

    /* Allocate an initial cache large enough for the first call. */
    if (!cache)
    {
        capacity = (n > 128) ? n : 128;
        cache = (seq_t *)calloc(capacity, sizeof(seq_t));
        if (!cache) abort();
    }

    /* Grow the cache if it cannot hold the requested index. */
    if (n > capacity)
    {
        size_t newcap = capacity;
        while (newcap < n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        capacity = newcap;
    }

    /* Initialise the first term a(1) = 1. */
    if (computed == 0)
    {
        cache[0] = (seq_t)1;
        computed = 1;
    }

    /* Compute any missing terms up to n (inclusive). */
    for (idx_t i = computed; i < n; ++i)
    {
        /* a(i+1) is stored at index i. */
        seq_t prev = cache[i - 1];          /* a(i)     */
        if ((prev & (seq_t)1) != 0)          /* odd?     */
            cache[i] = ((seq_t)3 * prev) + (seq_t)i;   /* (n-1)     */
        else
            cache[i] = prev / (seq_t)2;
    }
    computed = n;

    return cache[n - 1];
}
