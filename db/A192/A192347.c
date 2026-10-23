/*
 * A192347  Coefficient of x in the reduction (by x^2->x+1) of polynomial
 *          p(n,x) identified in Comments.
 * https://oeis.org/A192347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+6*a(n-2)-2*a(n-3)-a(n-4) Initial values: a(1)=0,
 *            a(2)=1, a(3)=2, a(4)=11
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* 0‑based array storing a(1) at cache[0] */
static idx_t  cur_size = 0;   /* number of terms stored in cache  */
static idx_t  capacity  = 0;  /* allocated size of cache           */

/* Ensure the cache is initialized with the first four terms. */
static void init_cache(void)
{
    if (cache == NULL) {
        capacity = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * capacity);
        cache[0] = (seq_t)0;   /* a(1) */
        cache[1] = (seq_t)1;   /* a(2) */
        cache[2] = (seq_t)2;   /* a(3) */
        cache[3] = (seq_t)11;  /* a(4) */
        cur_size = 4;
    }
}

/* Grow the cache to at least new_cap elements. */
static void grow_cache(idx_t new_cap)
{
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    /* realloc success is assumed in the test environment */
    cache = new_cache;
    capacity = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)            /* behaviour below the offset is irrelevant */
        return (seq_t)0;

    init_cache();

    /* Ensure the cache can hold the requested index. */
    if (n > capacity) {
        idx_t new_cap = capacity ? capacity * 2 : 4;
        while (new_cap < n)
            new_cap *= 2;
        grow_cache(new_cap);
    }

    /* Compute terms up to n-1 (zero‑based). */
    if (cur_size < n) {
        for (idx_t i = cur_size; i < n; ++i) {
            /* i >= 4 corresponds to n >= 5 */
            if (i >= 4) {
                cache[i] = 2 * cache[i - 1]
                         + 6 * cache[i - 2]
                         - 2 * cache[i - 3]
                         -   cache[i - 4];
            }
        }
        cur_size = n;
    }

    return cache[n - 1];   /* a(1) is stored at cache[0] */
}
