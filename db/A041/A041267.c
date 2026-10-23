/*
 * A041267  Denominators of continued fraction convergents to sqrt(146).
 * https://oeis.org/A041267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 290*a(n-2) - a(n-4), with a(0)=1, a(1)=12, a(2)=289,
 *            a(3)=3480
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* initialise cache with the first four terms if not already done */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)12;
        cache[2] = (seq_t)289;
        cache[3] = (seq_t)3480;
    }

    if (n < cache_len)
        return cache[n];

    /* grow cache to hold up to term n */
    idx_t new_len = n + 1;
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);

    /* compute all missing terms up to n using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = (seq_t)290 * cache[i - 2] - cache[i - 4];
        cache[i] = val;
    }

    cache_len = new_len;
    return cache[n];
}
