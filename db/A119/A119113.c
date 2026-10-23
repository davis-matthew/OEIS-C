/*
 * A119113  Triangular numbers composed of digits {1,3,5}.
 * https://oeis.org/A119113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = triangular number whose decimal representation uses only
 *            digits {1,3,5}
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *cache = NULL;        /* cached triangular numbers with digits {1,3,5} */
static size_t cache_cap = 0;       /* current capacity of cache array */
static size_t cache_size = 0;      /* number of stored elements in cache */
static idx_t next_k = 1;           /* next triangular index to examine */

/* Return true iff x contains only digits 1,3,5 */
static bool allowed_digits(seq_t x)
{
    while (x > 0) {
        uint8_t d = (uint8_t)(x % 10);
        if (d != 1 && d != 3 && d != 5)
            return false;
        x /= 10;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Expand cache until we have at least n elements */
    while (cache_size < n) {
        /* Compute k(k+1)/2 using seq_t to avoid overflow of idx_t */
        seq_t k = (seq_t)next_k;
        seq_t t = k * (k + (seq_t)1) / (seq_t)2;
        if (allowed_digits(t)) {
            /* Ensure capacity */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 1024;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) exit(1); /* allocation failed */
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_size++] = t;
        }
        next_k++;
    }
    return cache[n - 1];
}
