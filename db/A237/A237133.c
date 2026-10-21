/*
 * A237133  Values of x in the solutions to x^2 - 3xy + y^2 + 19 = 0, where 0
 *          < x < y.
 * https://oeis.org/A237133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) - a(n-4); Initial values: a(1)=4, a(2)=5,
 *            a(3)=7, a(4)=11
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached sequence values */
static size_t  cache_len = 0;    /* number of values currently computed */
static size_t  cache_cap = 0;    /* allocated capacity of cache */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n < 1 */
    size_t idx = (size_t)(n - 1);            /* zero‑based index (n≥1) */

    /* Allocate or grow cache to hold up to idx */
    if (cache_cap <= idx) {
        size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
        if (new_cap <= idx) new_cap = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                  /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute sequence values up to idx if not already done */
    for (size_t i = cache_len; i <= idx; ++i) {
        if (i == 0)          cache[i] = (seq_t)4;
        else if (i == 1)     cache[i] = (seq_t)5;
        else if (i == 2)     cache[i] = (seq_t)7;
        else if (i == 3)     cache[i] = (seq_t)11;
        else                 cache[i] = 3 * cache[i - 2] - cache[i - 4];
    }
    cache_len = idx + 1;

    return cache[idx];
}
