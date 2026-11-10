/*
 * A085249  Terms k of A002977 such that both (k-1)/2 and (k-1)/3 are also
 *          terms of A002977.
 * https://oeis.org/A085249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th term k of A002977 (Klarner-Rado: 1 in; m in => 2m+1,
 *            3m+1 in) with (k-1)/2 and (k-1)/3 both in A002977; computed by
 *            generating A002977 and binary-searching
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 50 terms through n = 778 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A085249_grow(seq_t *old, idx_t oldlen, idx_t newlen)
{
    seq_t *q = mem_new(seq_t, (size_t)newlen);
    idx_t i;
    for (i = 0; i < newlen; i++) q[i] = 0;
    for (i = 0; i < oldlen; i++) q[i] = old[i];
    if (old) mem_free(old);
    return q;
}

static seq_t *A085249_v = NULL;
static idx_t A085249_len = 0, A085249_cap = 0, A085249_i2 = 0, A085249_i3 = 0;
static seq_t *A085249_r = NULL;
static idx_t A085249_rlen = 0, A085249_rcap = 0, A085249_p = 0;

static void A085249_extend(void)
{
    if (A085249_len == 0) {
        A085249_v = A085249_grow(NULL, 0, 16);
        A085249_cap = 16;
        A085249_v[0] = 1;
        A085249_len = 1;
        return;
    }
    if (A085249_len == A085249_cap) {
        A085249_v = A085249_grow(A085249_v, A085249_cap, A085249_cap * 2);
        A085249_cap *= 2;
    }
    seq_t x = A085249_v[A085249_i2] * 2 + 1;
    seq_t y = A085249_v[A085249_i3] * 3 + 1;
    seq_t m = (x < y) ? x : y;
    if (x == m) A085249_i2++;
    if (y == m) A085249_i3++;
    A085249_v[A085249_len++] = m;
}

static bool A085249_has(seq_t t)
{
    idx_t lo = 0, hi = A085249_len;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (A085249_v[mid] < t) lo = mid + 1;
        else hi = mid;
    }
    return lo < A085249_len && A085249_v[lo] == t;
}

seq_t a(idx_t n)
{
    while (A085249_rlen < n) {
        while (A085249_len <= A085249_p) A085249_extend();
        seq_t k = A085249_v[A085249_p];
        A085249_p++;
        if (k < 7) continue;
        seq_t d = k - 1;
        if (d % 6 != 0) continue;
        if (A085249_has(d / 2) && A085249_has(d / 3)) {
            if (A085249_rlen == A085249_rcap) {
                idx_t nc = A085249_rcap ? A085249_rcap * 2 : 16;
                A085249_r = A085249_grow(A085249_r, A085249_rlen, nc);
                A085249_rcap = nc;
            }
            A085249_r[A085249_rlen++] = k;
        }
    }
    return A085249_r[n - 1];
}
