/*
 * A199926  Number of sequences of n coin flips that win on the last flip, if
 *          the sequence of flips ends with (1,1,1,1).
 * https://oeis.org/A199926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) - a(n-5) for n > 7 with initial
 *            values: a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=1, a(6)=1, a(7)=2,
 *            a(8)=4
 * Range:     u8 n <= 15, u16 n <= 25, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache holds previously computed values.
       cache[i] holds a(i+1) (0‑based). */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;

    /* Bail out for n == 0 (not used by OEIS offset). */
    if (n == 0)
        return (seq_t)0;

    /* Convert n to a size_t for array indexing. */
    size_t N = (size_t)n;

    /* Grow cache if needed. */
    if (N > cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < N)
            new_cap = N;
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            new_cap * sizeof(seq_t));
        if (!new_cache)          /* allocation failure: terminate */
            exit(EXIT_FAILURE);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialize the first eight terms if the cache is new or
       not yet fully initialized. */
    if (cache_size < 8)
    {
        cache_size = 8;
        cache[0] = (seq_t)0; /* a(1) */
        cache[1] = (seq_t)0; /* a(2) */
        cache[2] = (seq_t)0; /* a(3) */
        cache[3] = (seq_t)1; /* a(4) */
        cache[4] = (seq_t)1; /* a(5) */
        cache[5] = (seq_t)1; /* a(6) */
        cache[6] = (seq_t)2; /* a(7) */
        cache[7] = (seq_t)4; /* a(8) */
    }

    /* Compute any missing terms up to N. */
    for (size_t i = cache_size; i < N; ++i)
    {
        /* i corresponds to a(i+1).  The recurrence is:
           a(k) = a(k-1) + a(k-2) + a(k-3) - a(k-5).  */
        seq_t val = cache[i - 1] + cache[i - 2] + cache[i - 3]
                    - cache[i - 5];
        cache[i] = val;
    }

    cache_size = N;
    return cache[N - 1];
}
