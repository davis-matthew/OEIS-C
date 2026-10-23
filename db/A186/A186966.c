/*
 * A186966  Number of 3-turn queen's tours on an n X n board summed over all
 *          starting positions.
 * https://oeis.org/A186966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + 5*a(n-4) - 4*a(n-5) + a(n-6)
 *            (empirical recurrence). Initial values: a(1)=0, a(2)=24,
 *            a(3)=296, a(4)=1304, a(5)=3808, a(6)=8832
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined – return 0. */
    if (n < 1) return (seq_t)0;

    static seq_t *cache  = NULL;
    static size_t  cap   = 0;          /* capacity of cache in elements   */
    static idx_t   high  = 0;          /* highest index currently stored  */

    /* Ensure the cache is large enough for indices up to n. */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) {              /* allocation failed */
            if (cache) free(cache);
            return (seq_t)0;
        }
        /* Copy already computed values (if any). */
        if (cache && high > 0)
            memcpy(new_cache, cache, (high + 1) * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cap   = new_cap;
    }

    /* Initialise the first six values if not yet done. */
    if (high == 0) {
        /* guarantee space for indices 1..6 */
        if (cap < 7) {
            free(cache);
            cache = (seq_t *)malloc(7 * sizeof(seq_t));
            if (!cache) return (seq_t)0;
            cap = 7;
        }
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)24;
        cache[3] = (seq_t)296;
        cache[4] = (seq_t)1304;
        cache[5] = (seq_t)3808;
        cache[6] = (seq_t)8832;
        high = 6;
    }

    /* Compute any missing terms up to n using the recurrence. */
    if (n > high) {
        for (idx_t i = high + 1; i <= n; ++i) {
            /* recurrence: 4*a(i-1) - 5*a(i-2) + 5*a(i-4) - 4*a(i-5) + a(i-6) */
            seq_t t1 = cache[i - 1] * (seq_t)4;
            seq_t t2 = cache[i - 2] * (seq_t)5;
            seq_t t4 = cache[i - 4] * (seq_t)5;
            seq_t t5 = cache[i - 5] * (seq_t)4;
            seq_t t6 = cache[i - 6];
            cache[i] = t1 + t4 + t6 - t2 - t5;
        }
        high = n;
    }

    return cache[n];
}
