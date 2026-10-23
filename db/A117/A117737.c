/*
 * A117737  Numbers such that the (positive) difference between the sum of the
 *          digits and the product of the digits is a prime number.
 * https://oeis.org/A117737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product, digit_sum, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers such that |digit_sum(n) – digit_product(n)| is prime. */
/* This function builds the sequence incrementally and memoises it. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_sum.h"
#include "../utils/digit_product.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Safety: the offset of the sequence is 1. */
    if (n == 0) return 0;

    /* Static cache state. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;       /* number of terms stored */
    static size_t cache_cap   = 0;       /* allocated slots */
    static seq_t current = 0;           /* last integer tested */

    /* Grow the cache until it contains at least n entries. */
    while (cache_size < (size_t)n) {
        /* Next candidate integer. */
        current += 1;

        /* Compute digit sum and product. */
        const seq_t sum = digit_sum(current);
        const seq_t prod = digit_product(current);

        /* Absolute difference between sum and product. */
        const seq_t diff = sum > prod ? sum - prod : prod - sum;

        /* Test whether the difference is a prime > 1. */
        if (diff > 1 && is_prime_big(diff)) {
            /* Ensure cache capacity. */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = current;   /* store the term */
        }
    }

    /* Return the (1‑based) nth term. */
    return cache[n - 1];
}
