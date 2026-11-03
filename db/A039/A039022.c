/*
 * A039022  Numbers whose base-6 representation has the same number of 1's and
 *          4's.
 * https://oeis.org/A039022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th non‑negative integer whose base‑6 representation
 *            contains an equal number of 1’s and 4’s. Computed by exhaustive
 *            search
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9425 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

static inline bool balanced_ones_fours(seq_t x)
{
    unsigned count1 = 0, count4 = 0;
    while (x) {
        unsigned d = (unsigned)(x % 6);
        if (d == 1) ++count1;
        else if (d == 4) ++count4;
        x /= 6;
    }
    return count1 == count4;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* offset starts at 1 */

    /* Static cache of found values. */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t len = 0;
    static seq_t curr = 0;

    while ((idx_t)len < n) {
        if (balanced_ones_fours(curr)) {
            /* Grow the array if necessary. */
            if ((size_t)len == cap) {
                size_t newcap = cap ? cap << 1 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, newcap);
                if (!tmp) abort();                /* unlikely, but safe */
                cache = tmp;
                cap = newcap;
            }
            cache[len++] = curr;
        }
        ++curr;
    }
    return cache[(size_t)(n - 1)];
}
