/*
 * A221992  Number of binary arrays indicating the locations of trailing edge
 *          maxima of a random length-n 0..1 array extended with zeros and
 *          convolved with 1,4,6,4,1.
 * https://oeis.org/A221992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 51, u64 n <= 103,
 *            u128 n <= 207, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A221992: a(n) = a(n-1) + 2*a(n-3) - a(n-4) - a(n-6) + a(n-7)  (empirical)
   Initial values: a(1)=2, a(2)=3, a(3)=4, a(4)=6, a(5)=10, a(6)=15, a(7)=22
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base: a(0) is unused but kept as 0 for the recurrence */
    static seq_t *cache = NULL;   /* array of computed terms, 0‑based */
    static size_t cache_size = 0; /* number of elements allocated */

    if (n == 0U)                            /* not needed for OEIS offset but safe */
        return 0U;

    /* ---- initialise the cache with known values ---- */
    if (cache == NULL)
    {
        cache_size = 8U;                    /* indices 0..7 are stored */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0U;              /* allocation failure – unlikely */

        cache[0] = 0U;                      /* dummy value for a(0) */
        cache[1] = 2U;                      /* a(1)   */
        cache[2] = 3U;                      /* a(2)   */
        cache[3] = 4U;                      /* a(3)   */
        cache[4] = 6U;                      /* a(4)   */
        cache[5] = 10U;                     /* a(5)   */
        cache[6] = 15U;                     /* a(6)   */
        cache[7] = 22U;                     /* a(7)   */
    }

    /* ---- return if already computed ---- */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* ---- expand cache if necessary ---- */
    size_t old_size = cache_size;
    size_t new_size = (size_t)n + 1U;       /* need space up to index n */
    if (new_size > cache_size)
    {
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0U;                /* allocation failure – unlikely */
        cache = tmp;
        cache_size = new_size;
    }

    /* ---- compute missing terms using the recurrence ---- */
    for (size_t i = old_size; i <= (size_t)n; ++i)
    {
        /* recurrence: a(n) = a(n-1) + 2*a(n-3) - a(n-4) - a(n-6) + a(n-7) */
        seq_t val = cache[i - 1];
        val += (seq_t)2 * cache[i - 3];
        val -= cache[i - 4];
        val -= cache[i - 6];
        val += cache[i - 7];
        cache[i] = val;
    }

    return cache[(size_t)n];
}
