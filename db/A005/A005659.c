/*
 * A005659  If k appears so do 2k-2 and 3k-3. (duplicates omitted.)
 * https://oeis.org/A005659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sorted merge (Hamming-style) of the set S generated from 3 by k
 *            -> 2k-2 and k -> 3k-3, duplicates omitted; a(n) = n-th element
 *            of S after 3
 * Range:     u8 n <= 58, u16 n <= 4512, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9740 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

seq_t a(idx_t n) {
    seq_t *s = alloc_zero(n + 1);
    idx_t i = 0, j = 0;
    s[0] = 3;
    for (idx_t t = 1; t <= n; t++) {
        seq_t x = s[i] * 2 - 2;
        seq_t y = s[j] * 3 - 3;
        bool xo = (x / 2 != s[i] - 1);
        bool yo = (y / 3 != s[j] - 1);
        if (xo && yo) break;
        if (yo || (!xo && x < y)) {
            s[t] = x;
            i++;
        } else if (xo || y < x) {
            s[t] = y;
            j++;
        } else {
            s[t] = x;
            i++;
            j++;
        }
    }
    seq_t r = s[n];
    release(s, n + 1);
    return r;
}
