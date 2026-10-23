/*
 * A025928  Expansion of 1/((1-2*x)*(1-3*x)*(1-4*x)*(1-11*x)).
 * https://oeis.org/A025928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1)-125*a(n-2)+310*a(n-3)-264*a(n-4) with initial
 *            values: a(0)=1, a(1)=20, a(2)=275, a(3)=3310
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise base terms if needed */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        assert(cache != NULL);
        cache[0] = 1;
        cache[1] = 20;
        cache[2] = 275;
        cache[3] = 3310;
    }

    /* extend the cache up to index n */
    while (cache_len <= n) {
        idx_t m = cache_len;               /* new index to compute */

        /* recurrence: a(m) = 20*a(m-1) - 125*a(m-2) + 310*a(m-3) - 264*a(m-4) */
        seq_t next = 20 * cache[m-1];      /* 20*a(m-1) */
        next += 310 * cache[m-3];          /* +310*a(m-3) */
        next -= 125 * cache[m-2];          /* -125*a(m-2) */
        next -= 264 * cache[m-4];          /* -264*a(m-4) */

        /* grow the cache by one element */
        seq_t *tmp = (seq_t *)realloc(cache, (cache_len + 1) * sizeof(seq_t));
        assert(tmp != NULL);
        cache = tmp;
        cache[cache_len] = next;
        cache_len++;
    }

    return cache[n];
}
