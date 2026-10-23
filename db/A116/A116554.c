/*
 * A116554  a(2*n+1) = 7*a(n), a(2*n+2) = 8*a(n) + a(n-1).
 * https://oeis.org/A116554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(2*n+1) = 7*a(n), a(2*n+2) = 8*a(n) + a(n-1) (a(-1)
 *            is understood to be 0)
 * Range:     u8 n <= 6, u16 n <= 62, u32 n <= 2061, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of allocated entries */

    /* If the requested index is beyond the cache, grow it and
       compute the new entries (including the base cases) */
    if (n >= cache_len) {
        idx_t old_len = cache_len;
        /* ensure space for at least (n+1) entries; grow by a factor of 2 */
        idx_t new_len = cache_len ? cache_len * 2 : 4;
        if (new_len <= n) new_len = n + 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();                  /* memory exhaustion */
        cache = tmp;
        cache_len = new_len;

        /* Compute the new indices from old_len up to new_len-1 */
        for (idx_t i = old_len; i < new_len; ++i) {
            /* Initialise the known starting values */
            if (i == 0) {
                cache[0] = (seq_t)1;
                continue;
            }
            if (i == 1) {
                cache[1] = (seq_t)7;
                continue;
            }
            if (i == 2) {
                cache[2] = (seq_t)8;
                continue;
            }

            /* Use the recurrence: odd i -> 2*n+1, even i -> 2*n+2 */
            if (i & 1U) {                     /* i is odd: a(2*n+1) */
                idx_t k = (i - 1) >> 1;       /* n = (i-1)/2 */
                cache[i] = (seq_t)7 * cache[k];
            } else {                          /* i is even: a(2*n+2) */
                idx_t k = (i - 2) >> 1;       /* n = (i-2)/2 */
                cache[i] = (seq_t)8 * cache[k] + cache[k - 1];
            }
        }
    }

    /* The value is already cached */
    return cache[n];
}
