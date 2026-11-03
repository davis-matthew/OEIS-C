/*
 * A039404  Numbers whose base-10 representation has the same nonzero number
 *          of 1's and 4's.
 * https://oeis.org/A039404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 7957, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9103 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers having equal non‑zero counts of 1 and 4 in decimal representation */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Test whether a value has the same nonzero number of 1's and 4's. */
static bool has_equal_1_and_4(seq_t x)
{
    unsigned char c1 = 0, c4 = 0;
    while (x > 0) {
        unsigned char d = (unsigned char)(x % 10);
        if (d == 1) ++c1;
        else if (d == 4) ++c4;
        x /= 10;
    }
    return (c1 > 0) && (c1 == c4);
}

/* External linkage required by the build system. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined below offset, harmless */

    static seq_t *cache      = NULL;
    static idx_t   cache_cap = 0;
    static idx_t   cache_len = 0;
    static seq_t   last_number = 0;

    if (cache_len < n) {
        /* Grow capacity if necessary. */
        idx_t needed_cap = (cache_cap == 0) ? 16 : cache_cap;
        while (needed_cap < n) needed_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, needed_cap);
        if (!tmp) return 0;      /* allocation failure -> short‑circuit */
        cache = tmp;
        cache_cap = needed_cap;

        /* Generate terms until we have at least n entries. */
        while (cache_len < n) {
            ++last_number;
            if (has_equal_1_and_4(last_number))
                cache[cache_len++] = last_number;
        }
    }
    return cache[n - 1];
}
