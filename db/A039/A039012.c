/*
 * A039012  Numbers whose base-5 representation has the same number of 1's and
 *          4's.
 * https://oeis.org/A039012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth number whose base‑5 representation contains the
 *            same number of 1’s and 4’s (n≥1, a(1)=0). Computed by a linear
 *            search and memoisation of previous results
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state between calls */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_checked = 0;   /* highest integer already examined */

    /* lazily allocate the cache */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* out of memory: abort gracefully */
    }

    /* grow the cache if needed */
    while (cache_len < (size_t)n) {
        if (cache_len == 0) {            /* insert the first term 0 */
            cache[0] = (seq_t)0;
            cache_len = 1;
            last_checked = (seq_t)0;
            continue;
        }

        /* find the next integer that satisfies the condition */
        seq_t val = last_checked + (seq_t)1;
        seq_t temp = val;
        unsigned int ones = 0, fours = 0;
        while (temp > (seq_t)0) {
            unsigned int digit = (unsigned int)(temp % (seq_t)5);
            if (digit == 1) ++ones;
            else if (digit == 4) ++fours;
            temp /= (seq_t)5;
        }

        if (ones == fours) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                seq_t *newp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!newp) return 0; /* out of memory */
                cache = newp;
            }
            cache[cache_len++] = val;
        }
        last_checked = val;
    }

    return cache[(size_t)n - 1];
}
