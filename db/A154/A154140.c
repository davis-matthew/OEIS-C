/*
 * A154140  Indices k such that 6 plus the k-th triangular number is a perfect
 *          square.
 * https://oeis.org/A154140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=2, a(2)=4, a(3)=19, a(4)=29,
   a(n)=6*a(n-2)-a(n-4)+2 for n>4. */
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n<1; return 0 as a safe placeholder. */
        return (seq_t)0;
    }

    /* Cached results.  cache_len is the number of terms currently stored.
       cache[0] corresponds to a(1). */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialise cache with the first four terms if this is the first call. */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_len);
        if (!cache) abort();          /* memory failure */
        cache[0] = (seq_t)2;          /* a(1) */
        cache[1] = (seq_t)4;          /* a(2) */
        cache[2] = (seq_t)19;         /* a(3) */
        cache[3] = (seq_t)29;         /* a(4) */
    }

    /* Return already computed value if possible. */
    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* Extend the cache to contain terms up to n. */
    seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * n);
    if (!new_cache) abort();
    cache = new_cache;

    /* Compute missing terms from the first missing index up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* i > 4, so recurrence is applicable. */
        seq_t term = (seq_t)6 * cache[i - 3]   /* a(i-2) */
                     - cache[i - 5]             /* a(i-4) */
                     + (seq_t)2;
        cache[i - 1] = term;
    }

    cache_len = n;
    return cache[n - 1];
}
