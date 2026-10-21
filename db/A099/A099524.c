/*
 * A099524  Expansion of 1/(1-5*x-x^3).
 * https://oeis.org/A099524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + a(n-3); a(0)=1, a(1)=5, a(2)=25
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A099524: coefficients of 1/(1-5x-x³) */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* Allocate or grow cache as necessary */
    if (n >= cache_len) {
        size_t new_size = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

        /* Ensure the first three terms are available */
        if (n >= 0) cache[0] = 1;
        if (n >= 1) cache[1] = 5;
        if (n >= 2) cache[2] = 25;

        /* Compute terms from the first new index onward */
        idx_t start = cache_len;
        if (start < 3) start = 3;
        for (idx_t i = start; i <= n; ++i) {
            cache[i] = (seq_t)5 * cache[i - 1] + cache[i - 3];
        }

        cache_len = new_size;
    }
    return cache[n];
}
