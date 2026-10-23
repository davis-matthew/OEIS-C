/*
 * A014897  a(1)=1, a(n) = 14*a(n-1) + n.
 * https://oeis.org/A014897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) + n, with a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache of computed terms */
static idx_t cache_size = 0;    /* number of terms stored in cache */

/* Compute the nth term of OEIS sequence A014897 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    /* Grow cache if needed */
    if (cache_size < n) {
        idx_t old_size = cache_size;
        idx_t new_size = n;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)new_size);
        if (!tmp) abort();        /* allocation failed */
        cache = tmp;

        if (old_size == 0) {      /* initialise first term */
            cache[0] = (seq_t)1;
            old_size = 1;
        }

        for (idx_t i = old_size; i < new_size; ++i) {
            /* i is zero‑based; a(n) is stored at cache[n-1] */
            cache[i] = cache[i - 1] * (seq_t)14 + (seq_t)(i + 1);
        }

        cache_size = new_size;
    }

    return cache[n - 1];
}
