/*
 * A078678  Number of binary strings with n 1's and n 0's avoiding zigzags,
 *          that is avoiding the substrings 101 and 010.
 * https://oeis.org/A078678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n*a(n-1) + (n-2)*a(n-2) + 2*(n-4)*a(n-3) -
 *            (n-4)*a(n-4)) / n with initial values a(0)=1, a(1)=2, a(2)=4,
 *            a(3)=8. This uses the recurrence from the OEIS entry
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 48, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Base values for n = 0..3 */
    static const seq_t base[4] = { (seq_t)1, (seq_t)2, (seq_t)4, (seq_t)8 };

    /* Cache of previously computed values */
    static seq_t *cache   = NULL;
    static idx_t  cache_sz = 0;

    /* Detect whether we can safely use a 128‑bit intermediate.  This
       holds for all integral seq_t types (uint8_t … uint64_t). */
    static const bool use128 = (sizeof(seq_t) <= 8);

    /* Handle small n directly */
    if (n < 4) {
        return base[n];
    }

    /* Initialise the cache on first use */
    if (!cache) {
        cache_sz = 4;
        cache    = (seq_t *)malloc(sizeof(seq_t) * cache_sz);
        if (!cache) return (seq_t)0;              /* out of memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)8;
    }

    /* Grow the cache to at least n */
    if (n >= cache_sz) {
        idx_t old_sz = cache_sz;
        cache_sz = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * cache_sz);
        if (!tmp) return (seq_t)0;               /* out of memory */
        cache = tmp;

        if (use128) {
            /* Use a signed 128‑bit intermediate for the recurrence.
               This branch is only taken for small integral types,
               therefore n is small enough that all intermediate values
               fit in 128 bits.  */
            for (idx_t i = old_sz; i <= n; ++i) {
                signed __int128 num = 0;
                num += (signed __int128)2 * (signed __int128)i * (signed __int128)cache[i-1];
                num += (signed __int128)(i-2) * (signed __int128)cache[i-2];
                num += (signed __int128)2 * (signed __int128)(i-4) * (signed __int128)cache[i-3];
                num -= (signed __int128)(i-4) * (signed __int128)cache[i-4];
                cache[i] = (seq_t)(num / (signed __int128)i);
            }
        } else {
            /* Use seq_t arithmetic; the seq_t type supports large
               integers for the arbitrary‑precision build.  */
            for (idx_t i = old_sz; i <= n; ++i) {
                seq_t term = (seq_t)2 * (seq_t)i * cache[i-1];
                term += (seq_t)(i-2) * cache[i-2];
                term += (seq_t)2 * (seq_t)(i-4) * cache[i-3];
                term -= (seq_t)(i-4) * cache[i-4];
                cache[i] = term / (seq_t)i;
            }
        }
    }

    return cache[n];
}
