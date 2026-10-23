/*
 * A182243  a(0)=0, a(n) = (a(n-1) AND n) + n.
 * https://oeis.org/A182243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = (a(n-1) AND n) + n, a(0) = 0
 * Range:     u8 n <= 127, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoisation cache */
static size_t cache_len = 0;  /* current cached length */

/* compute sequence value */
seq_t a(idx_t n)
{
    size_t needed = (size_t)n + 1;          /* number of elements required */

    if (needed > cache_len) {              /* enlarge cache if necessary */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();                 /* allocation failed */
        cache = tmp;

        if (cache_len == 0) {              /* initialise first element */
            cache[0] = 0;
            cache_len = 1;
        }

        /* compute missing values up to n */
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            cache[i] = (prev & (seq_t)i) + (seq_t)i;
        }
        cache_len = needed;
    }

    return cache[n];
}
