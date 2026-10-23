/*
 * A188970  Zero-one sequence based on (4n-3): a(A016813(k))=a(k);
 *          a(A004772(k))=1-a(k); a(1)=0.
 * https://oeis.org/A188970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 135, u16 n <= 135, u32 n <= 135, u64 n <= 135,
 *            u128 n <= 135, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A188970
   a(1) = 0
   For n > 1:
     if n ≡ 1 (mod 4)      a(n) = a((n + 3)/4)
     otherwise            a(n) = 1 – a(n – ⌊(n + 3)/4⌋)
   The values are memoised and the array is grown on demand. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base case: the x‑axis of the sequence is 1‑based. */
    if (n < 1)
        return 0;

    /* Static cache: the value a[i] is stored in cache[i] for all
       i that have already been computed. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of valid indices + 1 */

    /* Allocate or grow the cache to accommodate the required index. */
    if (n + 1 > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache)  /* Out of memory – abort. */
            abort();

        /* Initialise newly allocated entries. */
        if (cache_size == 0)
            new_cache[0] = 0;   /* dummy; a[0] is unused */

        /* Fill in the values that are now reachable. */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 1)
                new_cache[1] = 0;              /* a(1) = 0 */
            else if ((i & 3) == 1) {          /* i ≡ 1 (mod 4) */
                idx_t k = (i + 3) >> 2;       /* k = (i+3)/4 */
                new_cache[i] = new_cache[k];
            } else {                          /* other residue classes */
                idx_t k = i - ((i + 3) >> 2); /* k = i – floor((i+3)/4) */
                new_cache[i] = 1 - new_cache[k];
            }
        }
        cache = new_cache;
        cache_size = n + 1;
    }

    return cache[n];
}
