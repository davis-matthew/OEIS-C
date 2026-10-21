/*
 * A061278  a(n) = 5*a(n-1) - 5*a(n-2) + a(n-3) with a(1) = 1 and a(k) = 0 if
 *          k <= 0.
 * https://oeis.org/A061278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - a(n-2) + 1, with a(0)=0, a(1)=1 (equivalent to
 *            the given 3‑term recurrence a(n)=5*a(n-1)-5*a(n-2)+a(n-3) for
 *            n≥2)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   alloc_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_cache.h"

static seq_t *seq_cache = NULL;   /* cache of previously computed terms   */
static idx_t  cache_len = 0;      /* number of elements stored (max index+1)  */

seq_t a(idx_t n) {
    if (cache_len <= n) {
        idx_t  old_len = cache_len;          /* current cache size */
        idx_t  new_len = n + 1;              /* needed size */

        /* allocate a new array of the required size */
        seq_t *new_cache = alloc_cache(new_len);
        if (!new_cache) abort();

        /* copy old values into the new cache */
        for (idx_t i = 0; i < old_len; ++i) new_cache[i] = seq_cache[i];
        free(seq_cache);
        seq_cache = new_cache;

        /* initialise the base values if they are missing */
        if (old_len == 0) {          /* first term only known */
            seq_cache[0] = 0;
            if (new_len > 1) seq_cache[1] = 1;
            old_len = (new_len > 1) ? 2 : 1;
        } else if (old_len == 1) {   /* only a(0) present */
            seq_cache[1] = 1;
            old_len = 2;             /* a(1) is now stored */
        }

        /* compute remaining terms using the 2nd‑order recurrence */
        for (idx_t i = old_len; i < new_len; ++i) {
            seq_cache[i] = seq_cache[i-1] * 4 - seq_cache[i-2] + 1;
        }
        cache_len = new_len;
    }
    return seq_cache[n];
}
