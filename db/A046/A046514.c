/*
 * A046514  Numbers with multiplicative persistence value 5.
 * https://oeis.org/A046514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1312, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_persist5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A046514: Numbers with multiplicative persistence 5
 * by brute‑force enumeration.  The function keeps a static cache
 * of discovered terms so it can answer calls in any order. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_persist5.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but safe fallback */

    static seq_t *cache = NULL;
    static size_t cache_len  = 0;   /* number of terms stored */
    static size_t cache_cap  = 0;   /* allocated capacity */
    static uint64_t last_checked = 0;  /* last integer examined */

    /* Grow the cache until we have at least n terms. */
    while (cache_len < n) {
        uint64_t cand = last_checked + 1;
        seq_t val = (seq_t)cand;

        if (is_persist5(val)) {
            if (cache_len == cache_cap) {      /* extend storage */
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);             /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
        last_checked = cand;
    }
    return cache[n - 1];
}
