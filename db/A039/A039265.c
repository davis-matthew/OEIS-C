/*
 * A039265  Numbers whose base-12 representation has the same number of 7's
 *          and 8's.
 * https://oeis.org/A039265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Rule: a(n) is the nth non‑negative integer whose base‑12 representation
   contains an equal number of 7’s and 8’s. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static int base12_equal78(seq_t v) {
    unsigned c7 = 0, c8 = 0;
    if (v == 0) return 1;   /* 0 has no digits; counts equal */
    while (v > 0) {
        unsigned d = (unsigned)(v % 12);
        if (d == 7) c7++;
        else if (d == 8) c8++;
        v /= 12;
    }
    return c7 == c8;
}

static seq_t *a_cache = NULL;
static size_t a_len  = 0;
static size_t a_cap  = 0;

static void a_grow(void) {
    if (a_cap == 0) a_cap = 16;
    else a_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, a_cache, a_cap);
    a_cache = tmp;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;         /* invalid offset, but safe guard */

    /* Return previously found value if already computed */
    if ((size_t)n <= a_len) return a_cache[(size_t)n - 1];

    seq_t cand = (a_len == 0) ? 0 : (a_cache[a_len - 1] + 1);

    while ((size_t)n > a_len) {
        if (base12_equal78(cand)) {
            if (a_len == a_cap) a_grow();
            a_cache[a_len++] = cand;
        }
        cand++;
    }
    return a_cache[(size_t)n - 1];
}
