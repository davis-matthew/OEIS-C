/*
 * A047906  a(n+1) = a(n) - n (if n is odd), a(n+1) = a(n) * n (if n is even).
 * https://oeis.org/A047906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(1) = 1; for n ≥ 1 * if n is odd: a(n+1) = a(n) - n * if n is
 *            even: a(n+1) = a(n) * n
 * Range:     i8 n <= 8, i16 n <= 12, i32 n <= 20, i64 n <= 34, i128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Allocate a cache array of size N+1 (indices 0..N, 0 unused). */
static seq_t *allocate_cache(idx_t N)
{
#if defined(__cplusplus)
    return new seq_t[N + 1];
#else
    return (seq_t *)malloc((N + 1) * sizeof(seq_t));
#endif
}

/* Free a cache array allocated by allocate_cache. */
static void free_cache(seq_t *ptr)
{
#if defined(__cplusplus)
    delete[] ptr;
#else
    free(ptr);
#endif
}

/* Hash table of previously computed terms. */
static seq_t *cache = NULL;   /* cache[1] .. cache[cached] are valid */
static idx_t cached = 0;      /* highest n that has been computed */

/* Ensure that cache[n] is known for all 1 ≤ n ≤ N. */
static void ensure_cache(idx_t N)
{
    if (N < 1)
        return;            /* nothing to do for n < 1 */

    /* First call – allocate and initialise the base case. */
    if (cache == NULL)
    {
        cache = allocate_cache(N);
        cache[1] = (seq_t)1;   /* a(1) = 1 */
        cached = 1;

        /* Compute the missing terms up to N. */
        for (idx_t i = 2; i <= N; ++i)
        {
            idx_t prev = i - 1;
            seq_t prev_val = (seq_t)prev;
            if ((prev & 1) == 0)            /* prev even → multiply */
                cache[i] = cache[prev] * prev_val;
            else                            /* prev odd → subtract */
                cache[i] = cache[prev] - prev_val;
        }
    }
    else if (cached < N) /* Grow the cache if necessary. */
    {
        seq_t *new_cache = allocate_cache(N);
        /* Copy existing values. */
        for (idx_t i = 1; i <= cached; ++i)
            new_cache[i] = cache[i];

        free_cache(cache);
        cache = new_cache;

        /* Compute the missing terms. */
        for (idx_t i = cached + 1; i <= N; ++i)
        {
            idx_t prev = i - 1;
            seq_t prev_val = (seq_t)prev;
            if ((prev & 1) == 0)
                cache[i] = cache[prev] * prev_val;
            else
                cache[i] = cache[prev] - prev_val;
        }
        cached = N;
    }
}

/* External function required by the build system. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1, return 0 as default */
    ensure_cache(n);
    return cache[n];
}
