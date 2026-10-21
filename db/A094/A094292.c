/*
 * A094292  Number of (s(0), s(1), ..., s(n)) such that 0 < s(i) < 5 and |s(i)
 *          - s(i-1)| <= 1 for i = 1,2,...,n, s(0) = 2, s(n) = 4.
 * https://oeis.org/A094292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) - 2*a(n-3) + a(n-4); Initial values:
 *            a(0)=0, a(1)=0, a(2)=1, a(3)=3
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoization cache: static array of already-computed values. */
    static seq_t *cache      = NULL;   /* pointer to cached values        */
    static idx_t cache_size = 0;       /* number of terms stored          */

    /* Initialise the first four terms on the first call. */
    if (!cache) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 4);
        if (!cache) return (seq_t)0;   /* PAM: out-of-memory guard */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)3;
        cache_size = 4;
    }

    /* If the requested term is already cached, return it. */
    if (n < cache_size)
        return cache[n];

    /* Expand the cache to accommodate up to index n. */
    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0;         /* PAM: out-of-memory guard */
    cache = tmp;

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = cache[i - 3];
        seq_t t4 = cache[i - 4];
        cache[i] = (seq_t)(4 * t1 - 3 * t2 - 2 * t3 + t4);
    }
    cache_size = n + 1;
    return cache[n];
}
