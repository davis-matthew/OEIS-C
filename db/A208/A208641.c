/*
 * A208641  Number of 6 X n 0..1 arrays with new values 0..1 introduced in row
 *          major order and no element equal to more than one of its immediate
 *          leftward or upward or right-upward antidiagonal neighbors.
 * https://oeis.org/A208641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 24, u64 n <= 56, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A208641:  Number of 6×n 0..1 arrays with new values 0..1 introduced in row major order
 *  and no element equal to more than one of its immediate leftward, upward, or right‑upward
 *  antidiagonal neighbors.
 *
 *  Formula used:   a(n) = 7*a(n-1) – 20*a(n-2) + 30*a(n-3) – 25*a(n-4) + 11*a(n-5) – 2*a(n-6)
 *  (valid for n > 9).  The first 9 values are hard‑coded and then the recurrence
 *  is applied for all larger n.  The result is memoised so each unique n is evaluated only once.
 */
#include <stdlib.h>

static seq_t *cache = NULL;          /* memoisation array, index 0 unused */
static idx_t cache_size = 0;         /* number of allocated elements (cache_size > n means a(n) cached) */

static const seq_t init_vals[10] = {
    0,          /* 0 unused – offset starts at 1 */
    32,         /* a(1) */
    233,        /* a(2) */
    851,        /* a(3) */
    2331,       /* a(4) */
    5572,       /* a(5) */
    12404,      /* a(6) */
    26508,      /* a(7) */
    55260,      /* a(8) */
    113427      /* a(9) */
};

seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_size = 10;                         /* indices 0 .. 9 will be set */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (cache == NULL) return (seq_t)0;
        for (idx_t i = 0; i < cache_size; ++i) cache[i] = init_vals[i];
    }

    if (n < cache_size) return cache[n];

    /* Ensure the cache can hold indices up to n */
    idx_t old_size = cache_size;
    if (n + 1 > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (tmp == NULL) {
            free(cache);
            cache = NULL;
            cache_size = 0;
            return (seq_t)0;
        }
        cache = tmp;
        cache_size = n + 1;
    }

    /* Evaluate new terms using the recurrence.  Start from the first uncached index. */
    for (idx_t i = old_size; i <= n; ++i) {
        /* Use 7*a(n-1) – 20*a(n-2) + 30*a(n-3) – 25*a(n-4) + 11*a(n-5) – 2*a(n-6) */
        seq_t val = 7 * cache[i - 1]
                  - 20 * cache[i - 2]
                  + 30 * cache[i - 3]
                  - 25 * cache[i - 4]
                  + 11 * cache[i - 5]
                  -  2 * cache[i - 6];
        cache[i] = val;
    }

    return cache[n];
}
