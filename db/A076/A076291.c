/*
 * A076291  a(n) = A002977(n+1) - A002977(n).
 * https://oeis.org/A076291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002977(n+1) - A002977(n), where A002977 is the
 *            Klarner-Rado sequence: b(1) = 1; if m is present then so are
 *            2m+1 and 3m+1 (two-pointer merge)
 * Range:     u8 n <= 64, u16 n <= 5111, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 117 terms through n = 9552 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A002977_grow(seq_t *old, idx_t oldlen, idx_t newlen)
{
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    idx_t i;
    for (i = 0; i < newlen; i++) p[i] = 0;
    for (i = 0; i < oldlen; i++) p[i] = old[i];
    if (old) mem_free(old);
    return p;
}

static seq_t *A002977_vals = NULL;
static idx_t A002977_len = 0, A002977_cap = 0;
static idx_t A002977_i2 = 0, A002977_i3 = 0;

static seq_t A002977_a(idx_t n) {
    if (A002977_len == 0) {
        A002977_vals = A002977_grow(NULL, 0, 16);
        A002977_cap = 16;
        A002977_vals[0] = 1;
        A002977_len = 1;
    }
    while (A002977_len < n) {
        if (A002977_len == A002977_cap) {
            A002977_vals = A002977_grow(A002977_vals, A002977_cap, A002977_cap * 2);
            A002977_cap *= 2;
        }
        seq_t x = A002977_vals[A002977_i2] * 2 + 1;
        seq_t y = A002977_vals[A002977_i3] * 3 + 1;
        seq_t m = (x < y) ? x : y;
        if (x == m) A002977_i2++;
        if (y == m) A002977_i3++;
        A002977_vals[A002977_len++] = m;
    }
    return A002977_vals[n - 1];
}

seq_t a(idx_t n) {
    seq_t hi = A002977_a(n + 1);
    seq_t lo = A002977_a(n);
    return hi - lo;
}
