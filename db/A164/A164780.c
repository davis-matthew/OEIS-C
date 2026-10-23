/*
 * A164780  Number of reduced words of length n in Coxeter group on 11
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A164780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) + 9*a(n-2) + 9*a(n-3) + 9*a(n-4) + 9*a(n-5) +
 *            9*a(n-6) + 9*a(n-7) - 45*a(n-8) for n >= 9 Initial values:
 *            a(0)=1, a(1)=11, a(2)=110, a(3)=1100, a(4)=11000, a(5)=110000,
 *            a(6)=1100000, a(7)=11000000, a(8)=109999945
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Computes the n-th term of OEIS sequence A164780.
   The function memoizes computed values and can be called
   in arbitrary order. */
seq_t a(idx_t n)
{
    /* Initialise the cache with the first nine values.  */
    if (cache_len == 0)
    {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);          /* allocation failed */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)11;
        cache[2] = (seq_t)110;
        cache[3] = (seq_t)1100;
        cache[4] = (seq_t)11000;
        cache[5] = (seq_t)110000;
        cache[6] = (seq_t)1100000;
        cache[7] = (seq_t)11000000;
        cache[8] = (seq_t)109999945;
        cache_len = 9;
    }

    /* If the requested term is already in the cache, return it. */
    if (n < cache_len)
        return cache[n];

    /* Grow the cache if necessary. */
    if ((size_t)n + 1 > cache_cap)
    {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(1);     /* allocation failed */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t sum =
            cache[i-1] * (seq_t)9 +
            cache[i-2] * (seq_t)9 +
            cache[i-3] * (seq_t)9 +
            cache[i-4] * (seq_t)9 +
            cache[i-5] * (seq_t)9 +
            cache[i-6] * (seq_t)9 +
            cache[i-7] * (seq_t)9 -
            cache[i-8] * (seq_t)45;
        cache[i] = sum;
    }
    cache_len = (size_t)n + 1;

    return cache[n];
}
