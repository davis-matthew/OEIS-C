/*
 * A044091  Numbers n such that string 2,4 occurs in the base 5 representation
 *          of n but not of n-1.
 * https://oeis.org/A044091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   contains_24_base5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the nth integer whose base‑5 representation
   contains the substring "24" but whose predecessor does not. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/contains_24_base5.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cached_len = 0;
    static size_t cached_cap = 0;

    /* Lazy allocation of cache */
    if (cache == NULL) {
        cached_cap = 128;
        cache = (seq_t *)malloc(cached_cap * sizeof(seq_t));
    }

    /* Grow cache until it contains at least n terms */
    if (cached_len < n) {
        seq_t cur = (cached_len == 0) ? 1 : cache[cached_len - 1] + 1;
        while (cached_len < n) {
            if (contains_24_base5(cur) && !contains_24_base5(cur - 1)) {
                if (cached_len == cached_cap) {
                    cached_cap *= 2;
                    cache = (seq_t *)realloc(cache, cached_cap * sizeof(seq_t));
                }
                cache[cached_len++] = cur;
            }
            ++cur;
        }
    }

    return cache[n - 1];
}
