/*
 * A139762  a(n)=Sum_digits_of_a(n-1)+Sum_digits_of(n).
 * https://oeis.org/A139762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = digit_sum(a(n-1)) + digit_sum(n) a(0) = 0 where
 *            digit_sum(x) is the sum of the decimal digits of x
 * Range:     u8 n <= 307, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"

/* sum of decimal digits of a value of type seq_t */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    /* initialise cache with a(0)=0 */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
        cap   = 1;
        len   = 1;
        cache[0] = 0;
    }

    /* grow cache if needed */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t prev    = cache[i - 1];
        seq_t sumPrev = digit_sum(prev);
        seq_t sumIdx  = digit_sum((seq_t)i);
        cache[i] = sumPrev + sumIdx;
    }
    len = (size_t)n + 1;

    return cache[(size_t)n];
}
