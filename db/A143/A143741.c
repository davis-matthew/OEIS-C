/*
 * A143741  Aliquot sequence starting at 72.
 * https://oeis.org/A143741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 72; a(n+1) = sum of proper divisors of a(n) (aliquot sum)
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   proper_divisor_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/proper_divisor_sum.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* initialise cache with initial term a(0) = 72 */
    if (cache == NULL) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) abort();        /* allocation failed */
        cache[0] = (seq_t)72;
    }

    /* extend cache up to requested index */
    while (cache_len <= (size_t)n) {
        seq_t prev = cache[cache_len - 1];
        seq_t next = proper_divisor_sum(prev);
        seq_t *tmp = (seq_t *)realloc(cache, (cache_len + 1) * sizeof(seq_t));
        if (tmp == NULL) abort();          /* allocation failed */
        cache = tmp;
        cache[cache_len] = next;
        ++cache_len;
    }

    return cache[(size_t)n];
}
