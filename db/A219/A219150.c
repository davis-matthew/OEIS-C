/*
 * A219150  Unchanging value maps: number of n X 2 binary arrays indicating
 *          the locations of corresponding elements unequal to no horizontal,
 *          antidiagonal or vertical neighbor in a random 0..1 n X 2 array.
 * https://oeis.org/A219150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 4*a(n-3) + a(n-5); Initial values:
 *            a(1)=2, a(2)=4, a(3)=9, a(4)=21, a(5)=49. This sequence
 *            satisfies the recurrence for all n >= 6
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 52, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Ensure we have at least the first 5 terms cached. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;              /* maximum index currently cached */

    /* Initial cache: terms 1..5 */
    if (cache == NULL) {
        cache_size = 5;
        cache = (seq_t*)malloc((cache_size + 1) * sizeof(seq_t));
        if (cache == NULL) return 0;          /* out of memory – return 0 as a safe value */
        cache[0] = 0;                         /* unused */
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)9;
        cache[4] = (seq_t)21;
        cache[5] = (seq_t)49;
    }

    /* If n is already cached, just return it. */
    if (n <= cache_size) return cache[n];

    /* Expand cache to hold up to index n.  */
    idx_t new_size = n + 1;                    /* include index 0 */
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL) return 0;                /* out of memory – safe fallback */
    cache = tmp;
    cache_size = n;

    /* Compute missing terms using the recurrence.  */
    for (idx_t i = cache_size; i >= 6; --i) {  /* skip recomputing the initial ones */
        /* The loop will run only if cache_size >= 6.  */
        cache[i] = 2 * cache[i-1] - cache[i-2] + 4 * cache[i-3] + cache[i-5];
    }

    /* We also need to compute terms 6..n if the growth of cache was smaller.
       The previous loop computed them in reverse order; just compute forward
       for clarity (this does not affect correctness). */
    for (idx_t i = 6; i <= cache_size; ++i) {
        /* Terms 1..5 are already set, so start at 6. */
        cache[i] = 2 * cache[i-1] - cache[i-2] + 4 * cache[i-3] + cache[i-5];
    }

    return cache[n];
}
