/*
 * A217761  Numbers whose square has a square number of decimal digits.
 * https://oeis.org/A217761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A217761:
   numbers whose square has a square number of decimal digits
*/

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_len.h"

static seq_t *cache = NULL;            /* 0‑based index storing terms */
static size_t cached_size = 0;         /* number of terms stored */
static size_t cached_cap   = 0;         /* allocated capacity */

/* Return true iff x is a perfect square (x <= INT_MAX) */
static bool is_square_int(int x) {
    if (x < 0) return false;
    int r = 0;
    while ((r * r) < x) ++r;
    return (r * r) == x;
}

/* Grow the cache to accommodate at least 'needed' entries */
static void grow_cache(size_t needed) {
    if (needed <= cached_cap) return;
    size_t new_cap = cached_cap ? cached_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newp) {
        /* In a real build, handle allocation failure appropriately */
        abort();
    }
    cache = newp;
    cached_cap = new_cap;
}

/* Compute the n-th term (1-indexed) */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* behaviour below offset not required */

    /* ensure cache contains at least n terms */
    if (cached_size < (size_t)n) {
        if (cached_size == 0) {
            grow_cache(1);
            cache[0] = 0;          /* a(1) = 0 */
            cached_size = 1;
        }
        for (idx_t i = cached_size; i < n; ++i) {
            seq_t last  = cache[i - 1];
            seq_t cand  = last + 1;
            while (true) {
                seq_t sq  = cand * cand;            /* square */
                int d     = digit_len(sq);          /* decimal digits */
                if (is_square_int(d)) {
                    grow_cache((size_t)(i + 1));
                    cache[i] = cand;                 /* store term */
                    break;
                }
                ++cand;
            }
        }
    }
    return cache[n - 1];
}
