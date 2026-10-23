/*
 * A152585  Generalized Fermat numbers: 12^(2^n) + 1, n >= 0.
 * https://oeis.org/A152585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 13; a(n) = (a(n-1)-1)^2 + 1 for n >= 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* initialize cache with the first term */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) return 0;          /* out of memory – undefined */
        cache[0] = 13;
        cache_len = 1;
    }

    /* grow cache if necessary and compute missing terms using the recurrence */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) return 0;           /* out of memory – undefined */
        cache = tmp;
        for (size_t i = cache_len; i < new_len; ++i) {
            seq_t prev = cache[i - 1];
            seq_t tmp = prev - 1;
            cache[i] = tmp * tmp + 1;
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
