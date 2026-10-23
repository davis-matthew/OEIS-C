/*
 * A039467  Numbers whose base-11 representation has the same nonzero number
 *          of 3's and 6's.
 * https://oeis.org/A039467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth integer whose base‑11 representation has the
 *            same non‑zero number of 3’s and 6’s
 * Range:     u8 n <= 4, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   check_property
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/check_property.h"

static seq_t *seq_cache = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;
static seq_t  seq_cur  = 1;   /* next candidate to test */

static void seq_grow(size_t needed)
{
    if (seq_cap < needed) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 4;
        if (new_cap < needed)
            new_cap = needed;
        if (seq_cache)
            seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        else
            seq_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* behaviour for n < 1 is irrelevant */

    if (n <= seq_len)            /* already computed */
        return seq_cache[n - 1];

    while (seq_len < (size_t)n) {
        seq_t candidate = seq_cur;
        if (check_property(candidate)) {
            seq_grow(seq_len + 1);
            seq_cache[seq_len++] = candidate;
        }
        ++seq_cur;
    }
    return seq_cache[n - 1];
}
