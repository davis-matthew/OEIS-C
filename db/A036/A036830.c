/*
 * A036830  Schoenheim bound L_1(n,n-4,n-5).
 * https://oeis.org/A036830
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n/(n-4) * a(n-1)), a(6)=3
 * Range:     u8 n <= 9, u16 n <= 25, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n-th term of OEIS A036830 (Schoenheim bound L_1).  */
seq_t a(idx_t n)
{
    /* No need to handle n < 6; return 0 for such indices. */
    if (n < 6)
        return (seq_t)0;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Ensure cache can hold up to index n */
    if (cache_size <= n) {
        idx_t new_size   = n + 1;                 /* include index n */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)          /* out of memory: fall back to 0 */
            return (seq_t)0;
        /* Zero the newly allocated entries (0 is never a real value). */
        for (idx_t i = cache_size; i < new_size; ++i)
            new_cache[i] = (seq_t)0;
        cache       = new_cache;
        cache_size  = new_size;
    }

    /* Initialise the base case a(6)=3 if not computed yet. */
    if (cache[6] == 0)
        cache[6] = (seq_t)3;

    /* Compute missing terms up to n. */
    for (idx_t i = 7; i <= n; ++i) {
        if (cache[i] != 0)          /* already computed in a previous call */
            continue;
        seq_t numerator = (seq_t)i * cache[i - 1];
        seq_t denom     = (seq_t)(i - 4);
        seq_t value     = numerator / denom;       /* floor division */
        if (numerator % denom != 0)
            value += 1;                             /* ceil correction */
        cache[i] = value;
    }

    return cache[n];
}
