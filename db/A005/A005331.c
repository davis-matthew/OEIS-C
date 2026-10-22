/*
 * A005331  Certain subgraphs of a directed graph (binomial transform of
 *          A005321).
 * https://oeis.org/A005331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} binomial(n,j)*A005321(j), computed by the
 *            equivalent DP h(0,0) = 1, h(j,c) = (2-2^c)*h(j-1,c) +
 *            (2^c-1)*h(j-1,c-1), a(n) = Sum_c h(n,c); h is kept as positive
 *            part P minus negative part N so all values stay nonnegative
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *P = NULL, *N = NULL, *M1 = NULL, *M2 = NULL, *Avals = NULL;
static idx_t cap = 0;
static idx_t cur = 0;   /* rows 0..cur computed */
static int inited = 0;

static seq_t *grow_buf(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = (seq_t *)malloc((size_t)newlen * sizeof(seq_t));
    idx_t i;
    for (i = 0; i < newlen; i++) p[i] = 0;
    for (i = 0; i < oldlen; i++) p[i] = old[i];
    if (old) free(old);
    return p;
}

static void ensure_cap(idx_t need) {
    if (need <= cap) return;
    idx_t nc = cap ? cap : 16;
    while (nc < need) nc *= 2;
    P = grow_buf(P, cap, nc);
    N = grow_buf(N, cap, nc);
    M1 = grow_buf(M1, cap, nc);
    M2 = grow_buf(M2, cap, nc);
    Avals = grow_buf(Avals, cap, nc);
    idx_t c;
    seq_t p2 = 1;
    for (c = 0; c < nc; c++) {
        if (c > 0) p2 = (seq_t)(p2 * 2);
        if (c >= cap) {
            M1[c] = (seq_t)(p2 - 1);
            M2[c] = (c >= 1) ? (seq_t)(p2 - 2) : (seq_t)0;
        }
    }
    cap = nc;
}

seq_t a(idx_t n) {
    if (!inited) {
        ensure_cap(16);
        P[0] = 1;
        N[0] = 0;
        Avals[0] = 1;
        cur = 0;
        inited = 1;
    }
    if (n <= cur) return Avals[n];
    ensure_cap(n + 1);
    while (cur < n) {
        idx_t j = cur + 1;
        idx_t c;
        for (c = j; c >= 1; c--) {
            seq_t np = M1[c] * P[c - 1] + M2[c] * N[c];
            seq_t nn = M1[c] * N[c - 1] + M2[c] * P[c];
            P[c] = np;
            N[c] = nn;
        }
        seq_t sp = 0, sn = 0;
        for (c = 0; c <= j; c++) {
            sp += P[c];
            sn += N[c];
        }
        Avals[j] = sp - sn;
        cur = j;
    }
    return Avals[n];
}
