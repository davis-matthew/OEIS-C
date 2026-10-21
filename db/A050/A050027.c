/*
 * A050027  a(n) = a(n-1) + a(m) for n >= 4, where m = 2*n - 2 - 2^(p+1) and p
 *          is the unique integer such that 2^p < n - 1 <= 2^(p+1), starting
 *          with a(1) = a(2) = a(3) = 1.
 * https://oeis.org/A050027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 51, u32 n <= 128, u64 n <= 508,
 *            u128 n <= 3097, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A050027: a(1)=a(2)=a(3)=1,
   a(n)=a(n-1)+a(2*n-2-2^(p+1)) (n≥4)
   where p = ⌊log₂(n‑2)⌋  (so 2^p < n-1 ≤ 2^(p+1)) */
#include <stdlib.h>
#include <limits.h>

seq_t a(idx_t n)
{
    /* Static cache for memoised values.  cache[i] holds a(i),
       with cache[0] unused. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n <= 3U)
        return 1;       /* base values */

    /* Grow cache to at least n */
    if (n > cache_size) {
        idx_t new_size = cache_size ? cache_size << 1 : 4;
        if (new_size < n) new_size = n;
        seq_t *new_cache = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
        if (!new_cache)
            return 0;   /* out of memory – not expected */

        cache = new_cache;
        /* Initialise base cases if just created */
        if (cache_size == 0) {
            cache[0] = 0;            /* unused */
            cache[1] = 1;
            cache[2] = 1;
            cache[3] = 1;
        }

        /* Compute new entries from old_size+1 up to new_size */
        for (idx_t i = cache_size + 1; i <= new_size; ++i) {
            if (i <= 3U) {          /* safety; base cases are already set */
                cache[i] = 1;
                continue;
            }
            /*  p = floor(log₂(i‑2))  → 2^p < i‑1 ≤ 2^(p+1)  */
            idx_t p = 0;
            idx_t t = i - 2U;            /* i‑2 ≥ 2 for i≥4 */
            while (t >>= 1)          /* count bits of i‑2, excluding the highest bit */
                ++p;
            idx_t power = ((idx_t)1) << (p + 1);   /* 2^(p+1) */
            idx_t m = 2U * i - 2U - power;         /* 1 ≤ m < i */
            cache[i] = cache[i - 1] + cache[m];
        }
        cache_size = new_size;
    }

    return cache[n];
}
