/*
 * A214160  Number of 0..5 colorings of an n X 3 array circular in the 3
 *          direction with new values 0..5 introduced in row major order.
 * https://oeis.org/A214160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A214160: number of 0..5 colorings of an n×3 array
   formula: a(n) = 84*a(n-1) - 945*a(n-2) + 1562*a(n-3)
   Initial values: a(1)=1, a(2)=18, a(3)=902 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache results so that calls in arbitrary order are fast.
       Indexing is 1‑based: cache[1] holds a(1). */
    static seq_t *cache = NULL;       /* array of size (cache_size+1) */
    static idx_t cache_size = 0;     /* current maximum index stored */

    if (n < 1) return (seq_t)0;       /* undefined, but safe fallback */

    if (n > cache_size) {
        idx_t new_size = n;           /* we need indices 1..n inclusive */
        seq_t *new_cache = (seq_t*)realloc(cache, (new_size+1) * sizeof(seq_t));
        if (!new_cache) abort();      /* out of memory – terminate */

        /* If expanding from an empty cache, still need to set placeholders. */
        if (!cache) {
            new_cache[0] = 0;         /* unused slot for 0th index */
        }

        /* Ensure initial values are set correctly. */
        if (new_size >= 1) new_cache[1] = (seq_t)1;
        if (new_size >= 2) new_cache[2] = (seq_t)18;
        if (new_size >= 3) new_cache[3] = (seq_t)902;

        /* Compute missing terms using the recurrence. */
        for (idx_t i = 4; i <= new_size; ++i) {
            new_cache[i] =
                ((seq_t)84    * new_cache[i-1]) -
                ((seq_t)945   * new_cache[i-2]) +
                ((seq_t)1562  * new_cache[i-3]);
        }

        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
