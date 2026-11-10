/*
 * A063252  Numbers for which a mixed L/R binary rotation sequence will reach
 *          a fixed point sooner than either a pure only-left or only-right
 *          iteration.
 * https://oeis.org/A063252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n-th k >= 1 with A063251(k) < min(A048881(k),
 *            A063250(k)); computed by relaxation-based BFS distance table
 *            over k <= 32767
 * Range:     u8 n <= 29, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

#define MAXN 32767

static unsigned short terms_tab[MAXN + 1];
static unsigned terms_cnt = 0;
static int built = 0;

static unsigned blen_x(unsigned x) {
    unsigned L = 0;
    while (x) { L++; x >>= 1; }
    return L;
}

static void build_tab(void) {
    static unsigned char dist[MAXN + 1];
    static unsigned short lt[MAXN + 1], rt[MAXN + 1];
    unsigned x;
    int changed;
    if (built) return;
    built = 1;
    for (x = 1; x <= MAXN; ++x) {
        unsigned L = blen_x(x);
        unsigned mask = (1u << L) - 1u;
        lt[x] = (unsigned short)(((x << 1) & mask) | 1u);
        rt[x] = (unsigned short)((x >> 1) | ((x & 1u) << (L - 1)));
        dist[x] = (((x + 1) & x) == 0) ? 0 : 255;
    }
    dist[0] = 0;
    do {
        changed = 0;
        for (x = 1; x <= MAXN; ++x) {
            unsigned a1 = dist[lt[x]], a2 = dist[rt[x]];
            unsigned m = a1 < a2 ? a1 : a2;
            if (m != 255 && m + 1 < dist[x]) {
                dist[x] = (unsigned char)(m + 1);
                changed = 1;
            }
        }
    } while (changed);
    for (x = 1; x <= MAXN; ++x) {
        unsigned y = x + 1, pc = 0, r = 0, t = x;
        while (y) { pc += y & 1u; y >>= 1; }
        while (((t + 1) & t) != 0) { ++r; t >>= 1; }
        if ((int)dist[x] < (int)pc - 1 && dist[x] < r)
            terms_tab[terms_cnt++] = (unsigned short)x;
    }
}

seq_t a(idx_t n) {
    build_tab();
    if (n < 1 || n > terms_cnt) return 0;
    return (seq_t)terms_tab[n - 1];
}
