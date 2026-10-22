/*
 * A002873  The maximal number of partitions of {1..2n} that are invariant
 *          under a permutation consisting of n 2-cycles, and which have the
 *          same number of nonempty parts.
 * https://oeis.org/A002873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = max_k T(n,k), where T(n,k) = k*T(n-1,k) + T(n-1,k-1) +
 *            T(n-1,k-2), T(0,0) = 1, T(0,k) = 0 for k != 0 (e.g.f.
 *            exp(y*(e^x-1) + y^2*(e^(2x)-1)/2))
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   zeroed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/zeroed.h"

static seq_t *t_row = 0;   /* t_row[k] = T(t_done, k) */
static idx_t t_rowcap = 0;
static seq_t *t_ans = 0;   /* t_ans[m] = a(m) for m <= t_done */
static idx_t t_anscap = 0;
static idx_t t_done = 0;
static int t_init = 0;

static seq_t *t_regrow(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = zeroed(newlen);
    idx_t i;
    for (i = 0; i < oldlen; i++) p[i] = old[i];
    return p;
}

seq_t a(idx_t n) {
    if (!t_init) {
        t_rowcap = 64;
        t_row = zeroed(t_rowcap);
        t_row[0] = 1;
        t_anscap = 32;
        t_ans = zeroed(t_anscap);
        t_ans[0] = 1;
        t_done = 0;
        t_init = 1;
    }
    if (n <= t_done) return t_ans[n];

    if (2 * n + 3 > t_rowcap) {
        idx_t nc = t_rowcap;
        while (nc < 2 * n + 3) nc *= 2;
        t_row = t_regrow(t_row, t_rowcap, nc);
        t_rowcap = nc;
    }
    if (n + 1 > t_anscap) {
        idx_t nc = t_anscap;
        while (nc < n + 1) nc *= 2;
        t_ans = t_regrow(t_ans, t_anscap, nc);
        t_anscap = nc;
    }

    while (t_done < n) {
        idx_t m = t_done + 1;
        idx_t top = 2 * m;
        idx_t k;
        seq_t best = 0;
        for (k = top; k >= 1; k--) {
            seq_t v = t_row[k] * (seq_t)k + t_row[k - 1];
            if (k >= 2) v += t_row[k - 2];
            t_row[k] = v;
        }
        t_row[0] = 0;
        for (k = 1; k <= top; k++)
            if (t_row[k] > best) best = t_row[k];
        t_ans[m] = best;
        t_done = m;
    }
    return t_ans[n];
}
