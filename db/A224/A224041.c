/*
 * A224041  Number of 5 X n 0..1 arrays with antidiagonals unimodal and rows
 *          and diagonals nondecreasing.
 * https://oeis.org/A224041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 21, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) - a(n-6)
 * for n>9.  Initial terms:
 * a(1)=32, a(2)=144, a(3)=298, a(4)=488, a(5)=734,
 * a(6)=1064, a(7)=1505, a(8)=2091, a(9)=2864. */

#include <stdlib.h>
#include <stdint.h>

/* Initial values (0‑based index: init[0] = a(1)). */
static const seq_t init[9] = {
    32, 144, 298, 488, 734, 1064, 1505, 2091, 2864
};

/* Cache for computed terms.   cache[i] holds a(i+1). */
static seq_t *cache = NULL;   /* 0‑based array */
static idx_t cache_n = 0;     /* number of values currently stored */

seq_t a(idx_t n)
{
    /* Return initial values directly if applicable. */
    if (n <= 9)
        return init[n - 1];

    /* Initialise cache only once. */
    if (cache == NULL) {
        size_t sz = 9;
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (cache == NULL)
            abort();
        for (idx_t i = 0; i < 9; ++i)
            cache[i] = init[i];
        cache_n = 9;
    }

    /* If the value is already cached, return it. */
    if (n <= cache_n)
        return cache[n - 1];

    /* Grow the cache to size n (0‑based indexing). */
    size_t new_n = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, new_n * sizeof(seq_t));
    if (tmp == NULL)
        abort();
    cache = tmp;

    /* Compute missing terms using the recurrence. */
    /*   cache[i] corresponds to a(i+1).  Index i runs from cache_n to n-1. */
    for (idx_t i = cache_n; i < n; ++i) {
        /* Positive part: 6*a(i) + 20*a(i-2) + 6*a(i-4)   (indices i-1, i-3, i-5) */
        seq_t pos = ((seq_t)6) * cache[i - 1] +
                    ((seq_t)20) * cache[i - 3] +
                    ((seq_t)6) * cache[i - 5];

        /* Negative part: 15*a(i-1) + 15*a(i-3) + a(i-5)   (indices i-2, i-4, i-6) */
        seq_t neg = ((seq_t)15) * cache[i - 2] +
                    ((seq_t)15) * cache[i - 4] +
                    cache[i - 6];

        cache[i] = pos - neg;   /* difference is guaranteed non‑negative */
    }

    cache_n = n;
    return cache[n - 1];
}
