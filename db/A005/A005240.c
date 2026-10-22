/*
 * A005240  P-positions in Epstein's Put or Take a Square game.
 * https://oeis.org/A005240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed form. Retrograde analysis of the game graph
 *            C -> C +- A048760(C) on [0, L). 0 is a P-position. A position
 *            with a move to a P-position is N. A position whose two moves
 *            both lead to N-positions is P. L doubles until the P-positions
 *            below the returned bound stay the same between...
 * Range:     u8 n <= 13, u16 n <= 34, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static long long *plist = NULL;
static idx_t pcount = 0;
static unsigned char *last_st = NULL;
static long long last_L = 0;

static long long isq(long long x) {
    long long r = 0, b = 1;
    while (b * b <= x) b <<= 1;
    for (; b > 0; b >>= 1)
        if ((r + b) * (r + b) <= x) r += b;
    return r;
}

/* returns state array: 0 = unknown/draw, 1 = P, 2 = N */
static unsigned char *build(long long L) {
    unsigned char *st = (unsigned char *)calloc((size_t)L, 1);
    unsigned char *deg = (unsigned char *)malloc((size_t)L);
    uint32_t *queue = (uint32_t *)malloc((size_t)L * sizeof(uint32_t));
    long long c, qh = 0, qt = 0;

    for (c = 0; c < L; c++) deg[c] = 2;
    st[0] = 1;
    queue[qt++] = 0;
    while (qh < qt) {
        long long m = queue[qh++];
        unsigned char sm = st[m];
        long long r, p;
        /* predecessors c = m + r^2 with r^2 <= c < (r+1)^2, i.e. m <= 2r */
        r = (m + 1) / 2;
        if (r < 1) r = 1;
        for (; (p = m + r * r) < L; r++) {
            if (st[p]) continue;
            if (sm == 1) { st[p] = 2; queue[qt++] = (uint32_t)p; }
            else if (--deg[p] == 0) { st[p] = 1; queue[qt++] = (uint32_t)p; }
        }
        /* predecessor c = m - r^2 with r^2 <= c < (r+1)^2 */
        r = isq(m / 2);
        if (r >= 1 && m < 2 * r * r + 2 * r + 1) {
            p = m - r * r;
            if (!st[p]) {
                if (sm == 1) { st[p] = 2; queue[qt++] = (uint32_t)p; }
                else if (--deg[p] == 0) { st[p] = 1; queue[qt++] = (uint32_t)p; }
            }
        }
    }
    free(deg);
    free(queue);
    return st;
}

seq_t a(idx_t n) {
    long long L;
    if (n <= pcount) return (seq_t)plist[n - 1];
    if (last_st == NULL) {
        last_L = 1LL << 15;
        last_st = build(last_L);
    }
    L = last_L * 2;
    for (;;) {
        unsigned char *st = build(L);
        long long x = L / 16, c;
        idx_t cnt = 0;
        for (c = 0; c < last_L && c < x; c++)
            if (st[c] == 1 && last_st[c] != 1) { x = c; break; }
        for (c = 0; c < x; c++) if (st[c] == 1) cnt++;
        free(plist);
        plist = (long long *)malloc((size_t)(cnt + 1) * sizeof(long long));
        pcount = 0;
        for (c = 0; c < x; c++) if (st[c] == 1) plist[pcount++] = c;
        free(last_st);
        last_st = st;
        last_L = L;
        if (n <= pcount) return (seq_t)plist[n - 1];
        L *= 2;
    }
}
