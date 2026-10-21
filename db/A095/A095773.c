/*
 * A095773  a(1)=1, a(n) = 1 + a(n - a(a(a(n-1)))).
 * https://oeis.org/A095773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(n) = 1 + a(n - a(a(a(n-1)))) for n > 1
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined for n >= 1; a(0) is unused.  Recursive
   dependence is guaranteed to stay within the already computed
   range because the values are always < n. */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* cache[1..] holds computed values; index 0 is unused */
    static seq_t *cache   = NULL;   /* array of a(i) */
    static size_t cap     = 0;      /* current capacity of cache array */
    static idx_t known    = 0;      /* largest n that has been computed */

    if (n == 0)                /* undefined for n < 1; return 0 safely */
        return (seq_t)0;

    if (n > known) {
        /* Allocate or grow the cache to hold values up to n */
        ensure_cache(n, &cache, &cap);

        for (idx_t i = known + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = (seq_t)1;
            } else {
                /* t1 = a(i-1) */
                seq_t t1 = cache[i - 1];
                /* t2 = a(t1)  – first cast to idx_t index */
                idx_t  idx_t1  = (idx_t) t1;
                seq_t t2 = cache[idx_t1];
                /* t3 = a(t2) */
                idx_t  idx_t2  = (idx_t) t2;
                seq_t t3 = cache[idx_t2];
                /* next index: i - t3 */
                idx_t  idx_t3  = (idx_t) t3;
                idx_t  next    = i - idx_t3;
                cache[i] = (seq_t)1 + cache[next];
            }
        }
        known = n;
    }

    return cache[n];
}
