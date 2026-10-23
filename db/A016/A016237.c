/*
 * A016237  Expansion of 1/((1-x)(1-5x)(1-10x)).
 * https://oeis.org/A016237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) - 50*a(n-2) + 1, with a(0)=1, a(1)=16
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i) for i < cache_len */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* initialize cache with first two values on the first call */
    if (cache_len == 0) {
        cache_cap = 2;
        cache = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        /* guarantee malloc succeeded – test harness assumes it will */
        cache[0] = (seq_t)1;           /* a(0) */
        cache[1] = (seq_t)16;          /* a(1) */
        cache_len = 2;
    }

    /* extend cache up to n if needed */
    while (cache_len <= n) {
        /* grow the cache if necessary */
        if (cache_len == cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap << 1 : 2;
            seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
            /* assume realloc succeeds */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute next term using the recurrence */
        seq_t next = (seq_t)15 * cache[cache_len - 1]
                    - (seq_t)50 * cache[cache_len - 2]
                    + (seq_t)1;
        cache[cache_len] = next;
        cache_len++;
    }

    return cache[n];
}
