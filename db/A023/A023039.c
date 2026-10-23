/*
 * A023039  a(n) = 18*a(n-1) - a(n-2), with a(0) = 1 and a(1) = 9.
 * https://oeis.org/A023039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) - a(n-2), a(0) = 1, a(1) = 9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Return the nth term of OEIS sequence A023039. */
seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache = NULL;        /* array of a(0) ... a(cache_len-1) */
    static idx_t cache_len = 0;        /* number of terms currently cached */

    /* Grow cache if necessary. */
    if (n + 1 > cache_len) {
        /* Allocate a larger array. */
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (new_cache == NULL) {
            /* Out of memory – abort in a clean way. */
            abort();
        }
        cache = new_cache;

        /* Fill newly allocated slots. */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)9;
            } else {
                /* a(i) = 18*a(i-1) - a(i-2) */
                cache[i] = (seq_t)18 * cache[i - 1] - cache[i - 2];
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
