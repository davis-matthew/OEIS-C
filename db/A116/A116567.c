/*
 * A116567  a(n) = +1476*a(n-6) +46656*a(n-12).
 * https://oeis.org/A116567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 1476*a(n-6) + 46656*a(n-12), with a(0)=0, a(1)=1, a(2)=2,
 *            a(3)=2, a(4)=40, a(5)=228, a(6)=228, a(7)=1440, a(8)=4248,
 *            a(9)=4248, a(10)=60336, a(11)=336528
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;  /* memoisation array */
    static idx_t   cap   = 0;    /* current capacity of cache */

    /* Constant initial segment, valid up to n = 11 */
    static const seq_t init[12] = {
        (seq_t)0ULL,
        (seq_t)1ULL,
        (seq_t)2ULL,
        (seq_t)2ULL,
        (seq_t)40ULL,
        (seq_t)228ULL,
        (seq_t)228ULL,
        (seq_t)1440ULL,
        (seq_t)4248ULL,
        (seq_t)4248ULL,
        (seq_t)60336ULL,
        (seq_t)336528ULL
    };

    /* If n is within the initial segment just return it. */
    if (n < 12) {
        return init[(size_t)n];
    }

    /* Need to grow the cache if n is beyond current capacity. */
    if (n >= cap) {
        idx_t old_cap = cap;
        idx_t new_cap = n + 1;
        if (new_cap < 12) new_cap = 12;          /* ensure space for the 12 constants */

        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cap   = new_cap;

        /* Initialise the first 12 entries (if they are newly allocated). */
        if (old_cap < 12) {
            for (idx_t i = old_cap; i < 12 && i < cap; ++i) {
                cache[i] = init[i];
            }
        }

        /* Compute all missing terms up to n. */
        for (idx_t i = old_cap; i <= n; ++i) {
            if (i < 12) {        /* already initialised */
                continue;
            }
            cache[i] = (seq_t)1476 * cache[i - 6]
                     + (seq_t)46656 * cache[i - 12];
        }
    }

    return cache[n];
}
