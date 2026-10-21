/*
 * A208633  Number of n X 4 0..1 arrays with new values 0..1 introduced in row
 *          major order and no element equal to more than one of its immediate
 *          leftward or upward or right-upward antidiagonal neighbors.
 * https://oeis.org/A208633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) + a(n-3); initial values: a(1)=8,
 *            a(2)=23, a(3)=71
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for sequence values (1‑based index) */
static seq_t *seq_cache = NULL;
static idx_t cache_size = 0;

/* Ensure the cache holds values up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (seq_cache == NULL)
    {
        /* Allocate for indices 0..3. We ignore index 0. */
        size_t sz = 4;
        seq_cache = (seq_t *)realloc(seq_cache, sz * sizeof(seq_t));
        seq_cache[1] = 8;
        seq_cache[2] = 23;
        seq_cache[3] = 71;
        cache_size = 3;
    }

    if (n <= cache_size)
        return;

    /* Grow the cache to accommodate index n */
    size_t new_sz = (size_t)n + 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_sz * sizeof(seq_t));

    /* Compute new values using the recurrence */
    for (idx_t i = cache_size + 1; i <= n; ++i)
    {
        seq_t tmp = seq_cache[i-1] * 5;          /* 5*a(i-1)   */
        tmp += seq_cache[i-3];                   /* + a(i-3)   */
        tmp -= seq_cache[i-2] * 6;               /* - 6*a(i-2) */
        seq_cache[i] = tmp;
    }

    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined below offset, safe fallback */

    ensure_cache(n);
    return seq_cache[n];
}
