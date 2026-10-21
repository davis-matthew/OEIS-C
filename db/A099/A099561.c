/*
 * A099561  Sum C(n-3k,k-1), k=0..floor(n/4).
 * https://oeis.org/A099561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0}^{⌊n/4⌋} C(n-3k, k-1) Recurrence: a(n) = a(n-1)
 *            + 2*a(n-4) - a(n-5) - a(n-8) Initial values (by definition):
 *            a(0)=a(1)=a(2)=a(3)=0, a(4)=a(5)=a(6)=a(7)=1 The routine
 *            memoises all computed terms up to the largest required n and
 *            expands the cache on demand. C++ compilers accept the...
 * Range:     u8 n <= 22, u16 n <= 39, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of already computed values.  cache[0] … cache[cache_size-1] exist. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialise cache with the first 8 values if not done yet. */
    if (cache == NULL) {
        cache_size = 8;                        /* indices 0 … 7 need to exist */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return 0;           /* allocation failure → 0 as fallback */
        cache[0] = cache[1] = cache[2] = cache[3] = 0;
        cache[4] = cache[5] = cache[6] = cache[7] = 1;
    }

    /* Direct hit – return the already known value. */
    if (n < cache_size)
        return cache[n];

    /* Extend the cache up to n inclusive. */
    idx_t old_size = cache_size;
    idx_t new_size = n + 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (new_cache == NULL) return 0;           /* allocation failure → 0 as fallback */
    cache = new_cache;
    cache_size = new_size;

    /* Compute the new entries.  Old entries (0…old_size-1) stay untouched. */
    for (idx_t i = old_size; i <= n; ++i) {
        if (i < 4) {
            cache[i] = 0;                        /* by definition for 0…3 */
        } else if (i < 8) {
            cache[i] = 1;                        /* by definition for 4…7 */
        } else {
            /* Recurrence for i ≥ 8: a(i) = a(i-1) + 2*a(i-4) - a(i-5) - a(i-8) */
            seq_t val = cache[i - 1] + 2 * cache[i - 4];
            val = val - cache[i - 5];
            val = val - cache[i - 8];
            cache[i] = val;
        }
    }

    return cache[n];
}
