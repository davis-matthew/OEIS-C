/*
 * A075841  Numbers k such that 2*k^2 - 9 is a square.
 * https://oeis.org/A075841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2), a(1)=3, a(2)=15
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Initialize cache with first two terms */
    if (!cache) {
        /* Allocate indices 0..2 (index 0 unused) */
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        if (!cache) return (seq_t)0;         /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)15;
        cache_size = 2;
    }

    /* Return early if already computed */
    if ((size_t)n <= cache_size) {
        return cache[n];
    }

    /* Extend cache up to n inclusive */
    size_t target_size = (size_t)n + 1;          /* +1 to keep index n accessible */
    seq_t *new_cache = (seq_t *)realloc(cache, target_size * sizeof(seq_t));
    if (!new_cache) return (seq_t)0;              /* allocation failure */
    cache = new_cache;

    /* Compute new terms using recurrence */
    for (size_t idx = cache_size + 1; idx <= (size_t)n; ++idx) {
        cache[idx] = cache[idx - 1] * ((seq_t)6) - cache[idx - 2];
    }

    cache_size = (size_t)n;
    return cache[n];
}
