/*
 * A039192  Numbers whose base-11 representation has the same number of 4's
 *          and 8's.
 * https://oeis.org/A039192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = numbers whose base‑11 representation has equal number of
 *            4's and 8's
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   same4and8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/same4and8.h"

static seq_t *cache = NULL;
static size_t cap  = 0;
static size_t len  = 0;
static idx_t   last_checked = 0;

seq_t a(idx_t n) {
    /* Ensure we have at least n terms cached */
    while (len < n) {
        seq_t candidate = (seq_t)last_checked;
        if (same4and8(candidate)) {
            if (len == cap) {
                size_t new_cap = cap == 0 ? 16 : cap * 2;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) exit(EXIT_FAILURE); /* out of memory */
                cache = new_cache;
                cap = new_cap;
            }
            cache[len++] = candidate;
        }
        ++last_checked;
    }
    return cache[n - 1];
}
