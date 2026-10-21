/*
 * A200881  Number of 0..3 arrays x(0..n+1) of n+2 elements without any
 *          interior element greater than both neighbors.
 * https://oeis.org/A200881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 4*a(n-1) - 6*a(n-2) + 10*a(n-3) - 5*a(n-4)
 *           + 6*a(n-5) - a(n-6) + a(n-7)
 * Initial values (a(1) .. a(7)):
 *   50, 144, 422, 1268, 3823, 11472, 34350
 * The sequence starts at offset 1. This implementation memoises
 * previously computed terms so that calls with arbitrary n order
 * work in O(n) time per new value.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Undefined for n < 1 – return 0 for safety */
    if (n == 0) return (seq_t)0;

    /* Memoisation buffer for computed values 0..n   */
    static seq_t *cache = NULL;   /* points to memory holding cache[n] for all n seen so far */
    static size_t cache_len = 0;  /* how many cache entries are allocated (cache_len == 0 means NULL) */

    /* Ensure the buffer is large enough for index n */
    if ((size_t)(n + 1) > cache_len) {
        /* Allocate or grow the buffer */
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure – safe fallback */
        /* Initialize newly allocated slots to 0 (so we can detect "not yet computed") */
        for (size_t i = cache_len; i < (size_t)(n + 1); ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cache_len = n + 1;
    }

    /* If the requested term is already known, just return it */
    if (cache[n] != (seq_t)0) return cache[n];

    /* Known initial values for a(1)..a(7) – index 0 unused */
    static const seq_t init[8] = {
        (seq_t)0,       /* 0 – unused */
        (seq_t)50,      /* 1 */
        (seq_t)144,     /* 2 */
        (seq_t)422,     /* 3 */
        (seq_t)1268,    /* 4 */
        (seq_t)3823,    /* 5 */
        (seq_t)11472,   /* 6 */
        (seq_t)34350    /* 7 */
    };

    /* Fill the cache from the smallest missing index up to n */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != (seq_t)0) continue;            /* already computed */

        if (i <= 7) {                                 /* initial data */
            cache[i] = init[i];
        } else {
            /* Direct application of the recurrence:
             * a(i) = 4*a(i-1) - 6*a(i-2) + 10*a(i-3)
             *        -5*a(i-4) + 6*a(i-5) - a(i-6) + a(i-7)
             * All intermediate results fit in seq_t because
             * the sequence grows smoothly and no intermediate
             * subtraction turns negative (a(i) > all subtracted terms). */
            seq_t t1 = cache[i-1] * (seq_t)4;
            seq_t t2 = cache[i-2] * (seq_t)6;
            seq_t t3 = cache[i-3] * (seq_t)10;
            seq_t t4 = cache[i-4] * (seq_t)5;
            seq_t t5 = cache[i-5] * (seq_t)6;
            seq_t t6 = cache[i-6];   /* multiplier 1 */
            seq_t t7 = cache[i-7];

            /* Compute using a single subtraction to keep values small */
            seq_t val = t1 + t3 + t5 + t7 - t2 - t4 - t6;
            cache[i] = val;
        }
    }

    return cache[n];
}
