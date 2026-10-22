/*
 * A007695  Cardinalities of Sperner families on 1,...,n.
 * https://oeis.org/A007695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = 1 + (number of f-vectors (f_{-1}=1, f_0, ..., f_{n-1}) of
 *            simplicial complexes on at most n vertices). By Kruskal-Katona
 *            these are exactly the sequences with 0 <= f_0 <= n and f_k <=
 *            f_{k-1}^{(k)}, where for m = C(a_k,k) + C(a_{k-1},k-1) + ...
 *            (the k-cascade) the bound is m^{(k)} = C(a_k,k...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

#define MAXN 63

static unsigned long long Cb[MAXN + 2][MAXN + 2];
static bool cb_init = false;

static void init_cb(void) {
    int i, j;
    if (cb_init) return;
    cb_init = true;
    for (i = 0; i <= MAXN + 1; i++) {
        Cb[i][0] = 1;
        for (j = 1; j <= MAXN + 1; j++)
            Cb[i][j] = (i == 0) ? 0 : Cb[i - 1][j - 1] + Cb[i - 1][j];
    }
}

/* Kruskal-Katona upper bound on the number of (k+1)-faces given m k-faces */
static unsigned long long kk_bound(unsigned long long m, int k, int n) {
    unsigned long long rem = m, b = 0;
    int i = k, av = n;
    while (rem > 0 && i >= 1) {
        while (Cb[av][i] > rem) av--;
        rem -= Cb[av][i];
        b += Cb[av][i + 1];
        i--;
        av--;
        if (av < 0) av = 0;
    }
    return b;
}

static seq_t memo[MAXN + 1];
static bool have[MAXN + 1];

seq_t a(idx_t n) {
    seq_t total;
    seq_t *cur, *nxt;
    idx_t curlen, nl, m, j;
    int k, nn;

    if (n == 0) return (seq_t)2;
    if (n <= MAXN && have[n]) return memo[n];
    init_cb();
    nn = (int)n;

    /* total: void complex + {emptyset} */
    total = (seq_t)2;
    curlen = n;
    cur = alloc_zero(curlen + 1);
    for (m = 1; m <= curlen; m++) {
        cur[m] = (seq_t)1;
        total += cur[m];
    }

    for (k = 1; k < nn; k++) {
        nl = (idx_t)Cb[nn][k + 1];
        nxt = alloc_zero(nl + 1);
        for (m = 1; m <= curlen; m++) {
            unsigned long long b = kk_bound((unsigned long long)m, k, nn);
            if (b > nl) b = nl;
            if (b > 0) nxt[b] += cur[m];
        }
        for (j = nl; j >= 2; j--) nxt[j - 1] += nxt[j];
        for (j = 1; j <= nl; j++) total += nxt[j];
        release(cur, curlen + 1);
        cur = nxt;
        curlen = nl;
    }
    release(cur, curlen + 1);

    if (n <= MAXN) {
        memo[n] = total;
        have[n] = true;
    }
    return total;
}
