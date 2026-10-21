/*
 * A212675  Number of (w,x,y,z) with all terms in {1,...,n} and w >= |x-y| +
 *          |y-z|.
 * https://oeis.org/A212675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + 5*a(n-4) - 4*a(n-5) + a(n-6)
 *            Initial values: a(0)=0, a(1)=1, a(2)=14, a(3)=57, a(4)=158,
 *            a(5)=353
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* allocated array of computed values   */
static size_t cache_cap = 0;      /* number of slots currently allocated   */
static idx_t cache_computed = 0;  /* next index that needs to be generated */

seq_t a(idx_t n)
{
    /* Handle the first six indices directly */
    if (n < 6) {
        switch (n) {
            case 0: return (seq_t)0;
            case 1: return (seq_t)1;
            case 2: return (seq_t)14;
            case 3: return (seq_t)57;
            case 4: return (seq_t)158;
            case 5: return (seq_t)353;
            default: return (seq_t)0; /* unreachable */
        }
    }

    /* Ensure a buffer large enough for index n */
    if (cache == NULL) {
        cache_cap = (size_t)(n + 1);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure */

        /* initialise the first six terms */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)14;
        cache[3] = (seq_t)57;
        cache[4] = (seq_t)158;
        cache[5] = (seq_t)353;
        cache_computed = 6;
    } else if ((size_t)(n + 1) > cache_cap) {
        /* grow the buffer if needed */
        cache_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failure */
        cache = tmp;
    }

    /* Generate any missing values up to n */
    for (idx_t i = cache_computed; i <= n; ++i) {
        /* recurrence is defined only for i >= 6 */
        cache[i] = 4 * cache[i - 1]
                 - 5 * cache[i - 2]
                 + 5 * cache[i - 4]
                 - 4 * cache[i - 5]
                 + cache[i - 6];
    }
    cache_computed = n + 1;

    return cache[n];
}
