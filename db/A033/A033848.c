/*
 * A033848  Numbers whose prime factors are 2 and 11.
 * https://oeis.org/A033848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th smallest integer whose only prime factors are 2
 *            and 11
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 128, u64 n <= 552,
 *            u128 n <= 2286, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;
    static idx_t  ptr2       = 0;   /* index of next element to multiply by 2 */
    static idx_t  ptr11      = 0;   /* index of next element to multiply by 11 */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)22;          /* 2^1 * 11^1 */
        cache_len = 1;
    }

    /* generate terms until cache contains at least n values */
    while (cache_len < n) {
        seq_t next2  = cache[ptr2] * (seq_t)2;
        seq_t next11 = cache[ptr11] * (seq_t)11;
        seq_t next = (next2 < next11) ? next2 : next11;

        if (next == next2) ++ptr2;
        if (next == next11) ++ptr11;

        /* avoid double insertion (duplicates occur when next2 == next11) */
        if (next != cache[cache_len - 1]) {
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = next;
        }
    }

    return cache[n - 1];
}
