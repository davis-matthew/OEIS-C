/*
 * A207830  Positive multiples of 3 that contain the decimal digit 1.
 * https://oeis.org/A207830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   contains1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A207830: the nth positive multiple of 3 containing the digit 1
   (searching multiples of 3 and checking for the digit 1). */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains1.h"

/* Return the nth term (n >= 1) of A207830 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sentinel for invalid index */

    /* Static cache for already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t candidate = 3;     /* next multiple of 3 to test */

    /* Extend cache until it contains at least n terms */
    while (cache_len < n) {
        /* Find next multiple of 3 that contains digit 1 */
        while (!contains1(candidate)) {
            candidate += 3;
        }

        /* Grow cache array if necessary */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap == 0 ? 16 : cache_cap << 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        cache[cache_len++] = candidate;   /* store term */
        candidate += 3;                   /* advance to next multiple */
    }

    return cache[(size_t)(n - 1)];
}
