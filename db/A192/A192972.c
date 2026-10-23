/*
 * A192972  Coefficient of x in the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + a(n-3) + 2*a(n-4) - a(n-5) a(0) =
 *            0, a(1) = 1, a(2) = 3, a(3) = 12, a(4) = 33
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 40, u64 n <= 86, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ---------- memoized recurrence implementation ---------- */
static seq_t *cache = NULL;   /* cached values of a(0), a(1), ...            */
static idx_t  cache_len = 0;  /* number of cached entries (size of cache)      */

/* Ensure that cache contains values up to and including a(n)   */
static void ensure_cache(idx_t n)
{
    /* If we already have enough values, nothing to do. */
    if (cache_len > n)
        return;

    /* New size we need: at least n+1, but also at least 5
       so the initial five terms are allocated.                   */
    size_t new_len = (size_t)(n + 1);
    if (new_len < 5)
        new_len = 5;

    /* Reallocate cache array to the new size. */
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    cache = tmp;   /* naive allocation error handling omitted */

    /* If this is the first allocation, set the first five values. */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)12;
        cache[4] = (seq_t)33;
    }

    /* Compute any new terms from the current cache_len up to new_len-1. */
    for (idx_t i = cache_len; i < new_len; ++i) {
        /* Skip the first five indices (already set). */
        if (i < 5)
            continue;

        /* Recurrence: a(i) = 4*a(i-1) - 5*a(i-2) + a(i-3) + 2*a(i-4) - a(i-5) */
        seq_t val = cache[i-1] * (seq_t)4;          /* 4*a(i-1)   */
        val += cache[i-3];                          /* +a(i-3)    */
        val += cache[i-4] * (seq_t)2;               /* +2*a(i-4)  */
        val -= cache[i-2] * (seq_t)5;               /* -5*a(i-2)  */
        val -= cache[i-5];                          /* -a(i-5)    */

        cache[i] = val;
    }

    cache_len = new_len;
}

/* Public function that returns the nth term of A192972        */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
