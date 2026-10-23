/*
 * A190130  Numbers 1 through 10000 sorted lexicographically in octal
 *          representation (base 8).
 * https://oeis.org/A190130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) is the nth number in 1..10000 sorted lexicographically by
 *            its octal (base‑8) representation
 * Range:     u8 n <= 3, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   oct_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../utils/oct_cmp.h"

static seq_t *cache = NULL;
static const idx_t CACHE_SIZE = 10000;

seq_t a(idx_t n)
{
    if (!cache)
    {
        cache = (seq_t *)malloc(CACHE_SIZE * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failure */
        idx_t *order = (idx_t *)malloc(CACHE_SIZE * sizeof(idx_t));
        if (!order)
        {
            free(cache);
            return 0;
        }
        for (idx_t i = 0; i < CACHE_SIZE; ++i) order[i] = i + 1;
        qsort(order, CACHE_SIZE, sizeof(idx_t), oct_cmp);
        for (idx_t i = 0; i < CACHE_SIZE; ++i) cache[i] = order[i];
        free(order);
    }
    if (n < 1 || n > CACHE_SIZE) return 0;  /* out of defined range */
    return cache[n - 1];
}
