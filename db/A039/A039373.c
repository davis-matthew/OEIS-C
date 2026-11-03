/*
 * A039373  Numbers whose base-9 representation has the same nonzero number of
 *          2's and 4's.
 * https://oeis.org/A039373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth positive integer whose base-9 representation contains
 *            an equal non-zero number of digits 2 and 4
 * Range:     u8 n <= 18, u16 n <= 7662, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9657 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_checked = 21; /* last number checked, start from 22 */

    if (n == 0) return 0;

    if (cache == NULL) {
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
        if (cache == NULL) return 0;
    }

    while (cache_len < n) {
        last_checked++;                     /* new candidate */
        seq_t x = last_checked;

        int c2 = 0, c4 = 0;
        seq_t t = x;
        while (t > 0) {
            int d = (int)(t % 9);
            if (d == 2) c2++;
            else if (d == 4) c4++;
            t /= 9;
        }

        if (c2 == c4 && c2 > 0) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                if (tmp == NULL) return 0;
                cache = tmp;
            }
            cache[cache_len++] = x;
        }
    }

    return cache[n - 1];
}
