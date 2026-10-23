/*
 * A138653  a(n) = 3*a(n-1)-3*a(n-2)+a(n-3)+a(n-4).
 * https://oeis.org/A138653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1)-3*a(n-2)+a(n-3)+a(n-4); a(0)=1, a(1)=2, a(2)=4,
 *            a(3)=8
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 36, u64 n <= 73, u128 n <= 147,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoisation array          */
static size_t  cap   = 0;        /* currently allocated size   */
static idx_t   max_n = 0;        /* highest n for which cache[n] is valid */

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (!cache) {
        size_t init_cap = 4;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)8;
        cap    = init_cap;
        max_n  = 3;
    }

    /* expand cache if needed */
    if (n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;
    }

    /* return stored value if already computed */
    if (n <= max_n)
        return cache[n];

    /* compute missing values up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        cache[i] =
            3 * cache[i-1] - 3 * cache[i-2] + cache[i-3] + cache[i-4];
    }
    max_n = n;
    return cache[n];
}
