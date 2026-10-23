/*
 * A171853  Sum of the trapezoid weights of all peakless Motzkin paths of
 *          length n (n>=0).
 * https://oeis.org/A171853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A171853 – Sum of the trapezoid weights of all peakless Motzkin paths of length n (n ≥ 0).
    The sequence satisfies the recurrence  
        (n-1)*a(n) = (3*n-5)*a(n-1)
                     -(n-7)*a(n-2)
                     -7*a(n-3) -5*a(n-4) -7*a(n-5)
                     + n*a(n-6)
                     -(3*n-16)*a(n-7)
                     +(n-6)*a(n-8)          (n ≥ 9)
    with initial values
        a(0)=0, a(1)=0, a(2)=0, a(3)=1, a(4)=3,
        a(5)=8, a(6)=20, a(7)=49, a(8)=119.
    The function below computes a(n) for any n ≥ 0 in O(n) time,
    storing previously computed values for later calls.  */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base values for n = 0 … 8 */
    static const seq_t init[9] = {
        0, 0, 0, 1, 3, 8, 20, 49, 119
    };

    /* Lazily allocated cache of previously computed terms. */
    static seq_t *cache = NULL;        /* cache[0 … cached_max] holds a(k) */
    static idx_t  cached_max = 0;      /* maximum index that has been computed */

    /* Return base values directly. */
    if (n <= 8) {
        return init[(idx_t)n];
    }

    /* Allocate or extend the cache as needed. */
    if (cache == NULL) {
        /* First allocation: size n+1, initialise with base values. */
        cache = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (cache == NULL) return 0;      /* allocation failure */
        for (idx_t i = 0; i <= 8; ++i) cache[i] = init[i];
        cached_max = 8;
    } else if (cached_max < n) {
        /* Extend the cache to hold terms up to index n. */
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)(n + 1) * sizeof(seq_t));
        if (tmp == NULL) return 0;      /* reallocation failure */
        cache = tmp;
        /* No need to initialise the new entries yet; they will be
           computed below. */
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        /* Positive contribution to the numerator */
        seq_t pos = (seq_t)(3 * i - 5) * cache[i - 1]
                  + (seq_t)i * cache[i - 6]
                  + (seq_t)(i - 6) * cache[i - 8];

        /* Negative contribution (subtracted from pos) */
        seq_t neg = (seq_t)(i - 7) * cache[i - 2]
                  + (seq_t)7 * cache[i - 3]
                  + (seq_t)5 * cache[i - 4]
                  + (seq_t)7 * cache[i - 5]
                  + (seq_t)(3 * i - 16) * cache[i - 7];

        /* The numerator is pos - neg and is always divisible by (i-1). */
        cache[i] = (pos - neg) / (seq_t)(i - 1);
    }

    cached_max = n;
    return cache[n];
}
