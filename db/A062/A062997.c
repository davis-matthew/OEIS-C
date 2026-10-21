/*
 * A062997  Numbers whose sum of digits is strictly greater than its product
 *          of digits.
 * https://oeis.org/A062997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy, look
 * Formula:   a(n) = the n-th integer whose sum of decimal digits is strictly
 *            greater than the product of its decimal digits. First term a(1)
 *            = 10. The function produces the sequence by scanning the natural
 *            numbers, caching results so that arbitrary indices can be
 *            processed efficiently
 * Range:     u8 n <= 81, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   good_digit_product_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/good_digit_product_sum.h"

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;   /* number of terms stored */
    static uint64_t current = 10; /* current integer to test */

    /* initialize cache on first call */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* extend cache until reaching desired index n */
    while (cache_len < n) {
        /* find next valid number */
        while (!good_digit_product_sum(current))
            ++current;

        /* store it */
        if ((size_t)cache_len >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = (seq_t)current;
        ++current;
    }

    /* return the n‑th term (offset 1) */
    return cache[n - 1];
}
