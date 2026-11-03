/*
 * A045176  Numbers whose base-5 representation contains exactly one 0 and no
 *          2's.
 * https://oeis.org/A045176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th nonnegative integer whose base‑5 representation
 *            contains exactly one 0 and no 2's
 * Range:     u8 n <= 49, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static inline bool _valid(seq_t v) {
    if (v == 0) return true;          /* '0' has exactly one zero digit */
    seq_t t = v;
    int zero_cnt = 0;
    while (t > 0) {
        uint64_t d = (uint64_t)(t % 5);
        if (d == 2) return false;
        if (d == 0) zero_cnt++;
        t /= 5;
    }
    return zero_cnt == 1;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t size = 0;
    static size_t cap = 0;
    static seq_t cur = 0;            /* next candidate to test */

    while (size < (size_t)n) {
        if (_valid(cur)) {
            if (size == cap) {
                size_t newcap = cap ? cap * 2 : 16;
                seq_t *p = mem_resize(seq_t, cache, newcap);
                cache = p;
                cap = newcap;
            }
            cache[size++] = cur;
        }
        cur++;                       /* advance to next integer */
    }
    return cache[(size_t)(n - 1)];
}
