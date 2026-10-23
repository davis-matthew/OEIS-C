/*
 * A164559  a(n) = 6^n/3 - 1.
 * https://oeis.org/A164559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-1)+5, with a(1)=1 */
#include <stddef.h>
#include <stdlib.h>

/* Cache to store computed terms. */
/* seq_cache[0] holds a(1) */
static seq_t *seq_cache = NULL;
static size_t cache_cap   = 0;   /* number of allocated slots */
static idx_t cached_n     = 0;   /* largest n computed so far */

/* Return a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n == 0 */

    /* Grow the cache if necessary. */
    if ((size_t)n > cache_cap) {
        seq_t *new_cache = (seq_t *)realloc(seq_cache, (size_t)n * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* allocation failed */
        seq_cache = new_cache;
        cache_cap = (size_t)n;
    }

    /* Initialise cache on first call. */
    if (cached_n == 0) {
        seq_cache[0] = (seq_t)1;   /* a(1) */
        cached_n = 1;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cached_n + 1; i <= n; ++i)
        seq_cache[i - 1] = seq_cache[i - 2] * (seq_t)6 + (seq_t)5;

    cached_n = n;
    return seq_cache[n - 1];
}
