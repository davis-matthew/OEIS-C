/*
 * A124062  Number of ways to write n as an ordered sum of 1's, 2's and 3's
 *          such that no 2 precedes any 1.
 * https://oeis.org/A124062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3) - a(n-4)-a(n-5)-a(n-6) with initial
 *            values a(0)=1,a(1)=1,a(2)=2,a(3)=3,a(4)=5,a(5)=8. This
 *            recurrence holds for all n ≥ 6
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 56, u64 n <= 114,
 *            u128 n <= 230, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoization cache */
static size_t cache_size = 0;    /* number of cached entries */

/* Ensure cache contains a(0)..a(n).  Allocate or grow as necessary. */
static void ensure_cache(idx_t n)
{
    if (n < (idx_t)cache_size)
        return;                 /* already cached */

    /* New capacity: at least n+1, but also at least 6 for the base terms. */
    size_t new_cap = (size_t)(n + 1);
    if (new_cap < 6)
        new_cap = 6;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)             /* out of memory */
        abort();

    cache = new_cache;
    size_t old_cap = cache_size;
    cache_size = new_cap;

    /* Compute terms from old_cap .. new_cap-1 */
    for (size_t i = old_cap; i < new_cap; ++i) {
        if (i < 6) {            /* base values */
            switch (i) {
            case 0: cache[0] = 1; break;
            case 1: cache[1] = 1; break;
            case 2: cache[2] = 2; break;
            case 3: cache[3] = 3; break;
            case 4: cache[4] = 5; break;
            case 5: cache[5] = 8; break;
            }
        } else {
            cache[i] = cache[i-1] + cache[i-2] + cache[i-3];
            cache[i] -= cache[i-4] + cache[i-5] + cache[i-6];
        }
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
