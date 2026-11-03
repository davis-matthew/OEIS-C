/*
 * A039016  Numbers whose base-6 representation has the same number of 0's and
 *          1's.
 * https://oeis.org/A039016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 120, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9880 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑6 representation has the same number of 0's and 1's. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool equal_zero_one(seq_t v) {
    int cnt0 = 0, cnt1 = 0;
    while (v > 0) {
        int d = (int)(v % 6);
        if (d == 0) cnt0++;
        else if (d == 1) cnt1++;
        v /= 6;
    }
    return cnt0 == cnt1;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static seq_t next = 0;

    if (n == 0) return 0;

    while (len < n) {
        next++;
        if (next < 2) continue;
        if (equal_zero_one(next)) {
            if (len + 1 > cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) return 0;
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = next;
        }
    }
    return cache[n - 1];
}
