/*
 * A036605  a(n) = a(n-2) + 2*a(n-3) + a(n-4).
 * https://oeis.org/A036605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A036605:  a(n) = a(n-2) + 2*a(n-3) + a(n-4),
   with a(0)=1, a(1)=4, a(2)=4, a(3)=7. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of computed values; built up to the maximum n seen so far */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 4);
        if (cache == NULL) {
            /* allocation failed – the build will not recover from this */
            abort();
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)7;
        cache_len = 4;
    }

    if (n < cache_len)
        return cache[n];

    /* grow cache to hold values up to n */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    if (tmp == NULL) abort();

    cache = tmp;
    /* compute all new values */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t temp = a3 + a3;  /* 2*a3, safe for integer and big‑int types */
        cache[i] = a2 + temp + a4;
    }

    cache_len = new_len;
    return cache[n];
}
