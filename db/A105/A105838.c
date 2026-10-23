/*
 * A105838  Nonnegative integers n such that 11*n^2 + 11*n + 1 is a square.
 * https://oeis.org/A105838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=39, a(3)=159, a(4)=15720 * Recurrence: a(n) =
 *            398*a(n-2)+198-a(n-4) for n>4 * (Computed iteratively with
 *            memoization)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of valid entries (last index computed) */

    /* n is 1‑based; guarantees n>=1 in the specification  */
    if (n < 1) return 0;          /* undefined, but safe */

    /* initialise cache for the first four terms when needed */
    if (cache == NULL) {
        cache_len = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * (cache_len + 1)); /* index 0 unused */
        if (cache == NULL) return 0;  /* allocation failed – unlikely in tests */
        cache[0] = 0;                 /* unused slot to keep indices simple */
        cache[1] = 0;
        cache[2] = 39;
        cache[3] = 159;
        cache[4] = 15720;
    }

    /* grow cache if we are asked for a larger n */
    if (n > cache_len) {
        idx_t new_len = n;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (new_len + 1));
        if (new_cache == NULL) return 0;  /* allocation failed – unlikely */
        cache = new_cache;

        /* compute missing terms using the recurrence
         *   a(k) = 398*a(k-2) + 198 - a(k-4)
         * start from the first term that needs the recurrence: k = 5
         */
        for (idx_t k = cache_len + 1; k <= new_len; ++k) {
            if (k < 5) continue;               /* already initialized */
            seq_t term = 398 * cache[k - 2] + (seq_t)198 - cache[k - 4];
            cache[k] = term;
        }
        cache_len = new_len;
    }

    return cache[n];
}
