/*
 * A080299  A014486-encoding of plane binary trees (Stanley's d) whose
 *          interior zigzag-tree (Stanley's c, i.e., tree obtained by
 *          discarding the outermost edges of the binary tree) is isomorphic
 *          to a valid plane binary tree (Stanley's d).
 * https://oeis.org/A080299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A080310(A014486(n)); A014486(n) is the n-th Dyck word
 *            (found by ballot-number unranking). A080310(m): m=0 gives 2;
 *            otherwise replace each 0 bit of m with 100 and append 10
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 30 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A080299_R 76
#define A080299_H 80

static uint64_t A080299_ways[A080299_R][A080299_H];
static int A080299_init = 0;

static void A080299_setup(void)
{
    int r, h;
    if (A080299_init) return;
    A080299_ways[0][0] = 1;
    for (r = 1; r < A080299_R; r++) {
        for (h = 0; h < A080299_H - 1; h++) {
            uint64_t v = A080299_ways[r - 1][h + 1];
            if (h > 0) v += A080299_ways[r - 1][h - 1];
            A080299_ways[r][h] = v;
        }
    }
    A080299_init = 1;
}

static seq_t A080299_dyck(idx_t n)
{
    int k, i, h;
    seq_t w = 0;
    uint64_t m;
    if (n == 0) return (seq_t)0;
    m = (uint64_t)(n - 1);
    for (k = 1; k < 37; k++) {
        uint64_t c = A080299_ways[2 * k][0];
        if (m < c) break;
        m -= c;
    }
    h = 0;
    for (i = 0; i < 2 * k; i++) {
        int rem = 2 * k - i - 1;
        int bit = 1;
        if (h > 0) {
            uint64_t cnt = A080299_ways[rem][h - 1];
            if (m < cnt) bit = 0;
            else m -= cnt;
        }
        if (bit) h++; else h--;
        w = (w << 1) | (seq_t)bit;
    }
    return w;
}

seq_t a(idx_t n)
{
    seq_t w, res, t;
    int len, i;
    A080299_setup();
    w = A080299_dyck(n);
    if (w == 0) return (seq_t)2;
    len = 0;
    t = w;
    while (t > 0) { t >>= 1; len++; }
    res = 0;
    for (i = len - 1; i >= 0; i--) {
        if ((w >> i) & 1) res = (res << 1) | 1;
        else res = (res << 3) | 4;
    }
    res = (res << 2) | 2;
    return res;
}
