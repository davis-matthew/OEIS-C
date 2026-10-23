/*
 * A173199  a(n) = a(n-1)+a(n-2)-Floor(a(n-3)/2)-Floor(a(n-8)/2); initial
 *          terms are 0, 1, 1, 2, 3, 5, 7, 11.
 * https://oeis.org/A173199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(2)=1, a(3)=2, a(4)=3, a(5)=5, a(6)=7, a(7)=11;
 *            recurrence: a(n)=a(n-1)+a(n-2)-floor(a(n-3)/2)-floor(a(n-8)/2)
 *            for n≥8 (floor division is equivalent to right shift by 1 for
 *            unsigned types)
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t capacity = 0;   /* allocated size */
    static size_t size = 0;       /* number of initialized elements */

    /* initial terms */
    const seq_t init[8] = {0,1,1,2,3,5,7,11};

    /* first call: allocate cache and copy initial values */
    if (cache == NULL)
    {
        capacity = 8;
        cache = (seq_t*)malloc(capacity * sizeof(seq_t));
        for (size_t i = 0; i < 8; ++i)
            cache[i] = init[i];
        size = 8;
    }

    /* ensure capacity for index n */
    if ((idx_t)(capacity) <= n)
    {
        size_t new_cap = capacity;
        while ((idx_t)(new_cap) <= n)
            new_cap *= 2;   /* grow by factor 2 */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failure unlikely for target limits */
        cache = tmp;
        capacity = new_cap;
    }

    /* compute any missing terms up to n (inclusive) */
    if (size <= n)
    {
        for (idx_t k = size; k <= n; ++k)
        {
            seq_t a1 = cache[(size_t)(k - 1)];
            seq_t a2 = cache[(size_t)(k - 2)];
            seq_t a3 = cache[(size_t)(k - 3)];
            seq_t a8 = cache[(size_t)(k - 8)];
            seq_t val = a1 + a2;
            val -= (a3 >> 1);   /* floor(a(k-3)/2) */
            val -= (a8 >> 1);   /* floor(a(k-8)/2) */
            cache[(size_t)k] = val;
        }
        size = (size_t)(n + 1);
    }

    return cache[(size_t)n];
}
