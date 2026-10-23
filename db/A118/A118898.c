/*
 * A118898  Number of binary sequences of length n containing exactly one
 *          subsequence 0000.
 * https://oeis.org/A118898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A118898.
   Formula (recurrence):
   a(n) = 2*a(n-1) + a(n-2) - a(n-4) - 4*a(n-5) - 3*a(n-6) - 2*a(n-7) - a(n-8)
   for n >= 8, with initial values
   a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=2, a(6)=5, a(7)=12, a(8)=28. */

#include <stdlib.h>
#include <string.h>

/* Base values for indices 0..8. */
static const seq_t init_vals[9] = {0,0,0,0,1,2,5,12,28};

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* memoised values          */
    static idx_t cache_size  = 0;       /* number of stored values  */

    /* If value already known, return it immediately. */
    if (n < cache_size)
        return cache[n];

    /* Need to enlarge cache to accommodate up to index n. */
    idx_t new_size = n + 1;                     /* new number of elements */
    seq_t *new_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
    if (!new_cache)
        return (seq_t)0; /* fallback, though allocation failure should not happen */

    /* Copy previously computed values into the new cache. */
    if (cache != NULL) {
        memcpy(new_cache, cache, cache_size * sizeof(seq_t));
        free(cache);
    }

    /* Initialize base values (indices 0..8) in the new cache. */
    for (idx_t i = 0; i < 9 && i < new_size; ++i)
        new_cache[i] = init_vals[i];

    /* Compute any additional terms that are now needed. */
    idx_t start = (cache_size > 8) ? cache_size : 9; /* first index to compute */

    for (idx_t i = start; i <= n; ++i) {
        seq_t val = (seq_t)2 * new_cache[i-1] + new_cache[i-2];
        val -= new_cache[i-4];
        val -= (seq_t)4 * new_cache[i-5];
        val -= (seq_t)3 * new_cache[i-6];
        val -= (seq_t)2 * new_cache[i-7];
        val -= new_cache[i-8];
        new_cache[i] = val;
    }

    /* Store the new cache for future calls. */
    cache      = new_cache;
    cache_size = new_size;

    return cache[n];
}
