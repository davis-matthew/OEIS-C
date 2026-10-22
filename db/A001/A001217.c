/*
 * A001217  Sorted list of orders of Weyl groups of types A_n, B_n, D_n, E_n,
 *          F_4, G_2.
 * https://oeis.org/A001217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   sorted union of A000142 ((n+1)!), A000165 (2^n*n!), A002866
 *            (2^(n-1)*n!), and {12, 1152, 51840, 2903040, 696729600} (G_2,
 *            F_4, E_6, E_7, E_8), found by merging the increasing streams and
 *            removing duplicates
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *T = NULL;
static idx_t cnt = 0, cap = 0;
static int inited = 0;
static seq_t cur[3];
static idx_t kk[3];
static int alive[3];
static int ci = 0;
static const unsigned long long CONSTS[5] = {12ULL, 1152ULL, 51840ULL, 2903040ULL, 696729600ULL};

static void push_val(seq_t v) {
    if (cnt == cap) {
        idx_t nc = cap ? cap * 2 : 64;
        seq_t *nt = (seq_t *)malloc(nc * sizeof(seq_t));
        for (idx_t i = 0; i < cnt; i++) nt[i] = T[i];
        if (T) free(T);
        T = nt;
        cap = nc;
    }
    T[cnt++] = v;
}

static int const_ok(int i) {
    seq_t c = (seq_t)CONSTS[i];
    return (unsigned long long)c == CONSTS[i];
}

/* multiply stream s by its ratio, killing it on overflow */
static void advance(int s) {
    idx_t m;
    if (s == 0) m = kk[0] + 2;          /* (k+1)! -> (k+2)! */
    else m = 2 * (kk[s] + 1);           /* ratio 2(k+1) for B and D */
    seq_t sm = (seq_t)m;
    if ((idx_t)(unsigned long long)sm != m || sm == 0) { alive[s] = 0; return; }
    seq_t nx = cur[s] * sm;
    if (nx / sm != cur[s]) { alive[s] = 0; return; }
    cur[s] = nx;
    kk[s]++;
}

static void init_state(void) {
    cur[0] = 1; kk[0] = 0; alive[0] = 1;   /* A_k: (k+1)!, k>=0 */
    cur[1] = 2; kk[1] = 1; alive[1] = 1;   /* B_k: 2^k k!, k>=1 */
    cur[2] = 1; kk[2] = 1; alive[2] = 1;   /* D_k: 2^(k-1) k!, k>=1 */
    ci = 0;
    while (ci < 5 && !const_ok(ci)) ci++;
    inited = 1;
}

static int gen_next(void) {
    int have = 0;
    seq_t m = 0;
    for (int s = 0; s < 3; s++) {
        if (alive[s] && (!have || cur[s] < m)) { m = cur[s]; have = 1; }
    }
    if (ci < 5) {
        seq_t c = (seq_t)CONSTS[ci];
        if (!have || c < m) { m = c; have = 1; }
    }
    if (!have) return 0;
    for (int s = 0; s < 3; s++) {
        if (alive[s] && cur[s] == m) advance(s);
    }
    if (ci < 5 && (seq_t)CONSTS[ci] == m) {
        ci++;
        while (ci < 5 && !const_ok(ci)) ci++;
    }
    if (cnt == 0 || T[cnt - 1] < m) push_val(m);
    return 1;
}

seq_t a(idx_t n) {
    if (!inited) init_state();
    while (cnt < n) {
        if (!gen_next()) break;
    }
    if (n >= 1 && n <= cnt) return T[n - 1];
    return 0;
}
