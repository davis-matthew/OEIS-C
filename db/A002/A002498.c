/*
 * A002498  Related to a highly composite sequence (A002497).
 * https://oeis.org/A002498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = A008475(A002497(n)). Computed by search: sort the
 *            elementary steps "add a new prime p" (cost p) and "raise p^k to
 *            p^(k+1)" (cost p^k*(p-1)) by cost/log(p). Steps "add 2" and
 *            "raise 2 to 4" have the same ratio, so they are merged into one
 *            step of cost 4. a(n) is the sum of the costs of the...
 * Range:     u8 n <= 15, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

typedef struct { long double r; uint64_t c; } a2498_ev;

static uint64_t *a2498_pre = 0;
static idx_t a2498_cnt = 0;

static int a2498_cmp(const void *x, const void *y) {
    const a2498_ev *u = (const a2498_ev *)x, *v = (const a2498_ev *)y;
    if (u->r < v->r) return -1;
    if (u->r > v->r) return 1;
    return 0;
}

static void a2498_build(idx_t need) {
    long double T = 16.0L;
    for (;;) {
        uint64_t L = (uint64_t)(T * (logl(T) + 2.0L)) + 16;
        unsigned char *comp = (unsigned char *)calloc((size_t)L + 1, 1);
        size_t cap = 1024, m = 0;
        a2498_ev *ev = (a2498_ev *)malloc(cap * sizeof(a2498_ev));
        uint64_t p, q;
        for (p = 2; p <= L; p++) {
            long double lp;
            uint64_t pk;
            if (comp[p]) continue;
            for (q = p * p; q <= L; q += p) comp[q] = 1;
            lp = logl((long double)p);
            if ((long double)p / lp > T) continue;
            if (m + 64 > cap) { cap *= 2; ev = (a2498_ev *)realloc(ev, cap * sizeof(a2498_ev)); }
            if (p == 2) {
                ev[m].r = 2.0L / lp; ev[m].c = 4; m++;
                pk = 4;
            } else {
                ev[m].r = (long double)p / lp; ev[m].c = p; m++;
                pk = p;
            }
            for (;;) {
                uint64_t cost = pk * (p - 1);
                long double r = (long double)cost / lp;
                if (r > T) break;
                if (m + 1 > cap) { cap *= 2; ev = (a2498_ev *)realloc(ev, cap * sizeof(a2498_ev)); }
                ev[m].r = r; ev[m].c = cost; m++;
                pk *= p;
            }
        }
        free(comp);
        if ((idx_t)m >= need) {
            size_t i;
            uint64_t s = 0;
            qsort(ev, m, sizeof(a2498_ev), a2498_cmp);
            if (a2498_pre) free(a2498_pre);
            a2498_pre = (uint64_t *)malloc((m + 1) * sizeof(uint64_t));
            a2498_pre[0] = 0;
            for (i = 0; i < m; i++) { s += ev[i].c; a2498_pre[i + 1] = s; }
            a2498_cnt = (idx_t)m;
            free(ev);
            return;
        }
        free(ev);
        T *= 2.0L;
    }
}

seq_t a(idx_t n) {
    if (n > a2498_cnt) a2498_build(n + n / 4 + 16);
    return (seq_t)a2498_pre[n];
}
