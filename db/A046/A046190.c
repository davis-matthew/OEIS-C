/*
 * A046190  Indices of octagonal numbers which are also hexagonal numbers.
 * https://oeis.org/A046190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 98*a(n-1) – a(n-2) – 32 Initial values: a(1) = 1, a(2) =
 *            63. This recurrence holds for n ≥ 3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[i] stores a(i+1)  */
static idx_t cache_len = 0;   /* number of entries currently stored */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n < 1, return 0 as sentinel */

    /* Grow cache if necessary */
    if (cache_len < n) {
        /* new size needed */
        idx_t new_len = n;
        if (cache == NULL) {
            cache = (seq_t*)malloc(new_len * sizeof(seq_t));
            cache_len = new_len;
        } else {
            cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
            cache_len = new_len;
        }

        /* initialize new entries */
        if (new_len >= 1) cache[0] = (seq_t)1;   /* a(1) */
        if (new_len >= 2) cache[1] = (seq_t)63;  /* a(2) */
        for (idx_t i = 2; i < new_len; ++i) {
            cache[i] = (seq_t)98 * cache[i-1] - cache[i-2] - (seq_t)32;
        }
    }

    return cache[n-1];
}
