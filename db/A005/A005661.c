/*
 * A005661  k in S implies 2k-2, 3k-3 in S.
 * https://oeis.org/A005661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 3586, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9829 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: S = closure of {5} under k -> 2k-2, 3k-3; merge the two image streams in increasing order (as for Hamming numbers), with duplicates removed */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *alloc_array(idx_t n) {
    seq_t *p = mem_new(seq_t, (n + 1));
    for (idx_t i = 0; i <= n; i++) {
        p[i] = 0;
    }
    return p;
}

static seq_t *grow_arr(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *r = mem_new(seq_t, newlen);
    idx_t i;
    for (i = 0; i < oldlen; i++) r[i] = old[i];
    if (old) mem_free(old);
    return r;
}

/* candidate 2x-2; returns false on wraparound in fixed-width types */
static bool A005661_c2(seq_t x, seq_t *out)
{
    seq_t c = x * 2 - 2;
    if ((c + 2) / 2 != x) return false;
    *out = c;
    return true;
}

static bool A005661_c3(seq_t x, seq_t *out)
{
    seq_t c = x * 3 - 3;
    if ((c + 3) / 3 != x) return false;
    *out = c;
    return true;
}

static seq_t *A005661_t = 0;
static idx_t A005661_len = 0, A005661_cap = 0;
static idx_t A005661_i2 = 0, A005661_i3 = 0;
static bool A005661_stuck = false;

seq_t a(idx_t n)
{
    if (A005661_t == 0) {
        A005661_cap = 64;
        A005661_t = alloc_array(A005661_cap);
        A005661_t[0] = 5;
        A005661_len = 1;
    }
    while (A005661_len < n && !A005661_stuck) {
        seq_t v2 = 0, v3 = 0, m;
        bool ok2 = A005661_c2(A005661_t[A005661_i2], &v2);
        bool ok3 = A005661_c3(A005661_t[A005661_i3], &v3);
        if (!ok2 && !ok3) { A005661_stuck = true; break; }
        if (ok2 && (!ok3 || v2 <= v3)) m = v2; else m = v3;
        if (ok2 && v2 == m) A005661_i2++;
        if (ok3 && v3 == m) A005661_i3++;
        if (A005661_len == A005661_cap) {
            idx_t nc = A005661_cap * 2;
            A005661_t = grow_arr(A005661_t, A005661_cap, nc);
            A005661_cap = nc;
        }
        A005661_t[A005661_len++] = m;
    }
    if (n > A005661_len) return A005661_t[A005661_len - 1];
    return A005661_t[n - 1];
}
