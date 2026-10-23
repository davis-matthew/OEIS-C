/*
 * A025969  Expansion of 1/((1-2*x)*(1-4*x)*(1-6*x)*(1-11*x)).
 * https://oeis.org/A025969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 23*a(n-1)-176*a(n-2)+532*a(n-3)-528*a(n-4) a(0) = 1, a(1)
 *            = 23, a(2) = 353, a(3) = 4603 Recurrence is valid for all n ≥ 4
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Static cache for memoisation.  It is thread‑unsafe but adequate
   for the intended use. */
static seq_t *cache = NULL;
static idx_t  cache_size = 0;

/* Ensure the cache contains values up to index n inclusive. */
static void ensure_cache(idx_t n)
{
    if (cache_size <= n) {
        /* Allocate or grow the cache. */
        idx_t newsize = n + 1;
        seq_t *newcache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!newcache) {
            /* On allocation failure terminate the program. */
            exit(1);
        }
        cache = newcache;

        /* Compute missing entries. */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)23;
            } else if (i == 2) {
                cache[i] = (seq_t)353;
            } else if (i == 3) {
                cache[i] = (seq_t)4603;
            } else {
                seq_t t1 = (seq_t)23   * cache[i - 1];
                seq_t t2 = (seq_t)176  * cache[i - 2];
                seq_t t3 = (seq_t)532  * cache[i - 3];
                seq_t t4 = (seq_t)528  * cache[i - 4];
                cache[i] = t1 - t2 + t3 - t4;
            }
        }
        cache_size = newsize;
    }
}

/* Public sequence function.  The function is external linkage, as
   required. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
