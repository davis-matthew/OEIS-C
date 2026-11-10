/*
 * A214267  Duplicate of A005437.
 * https://oeis.org/A214267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = A005437(n) = E(n, ceil(n/2)), E = Entringer numbers
 *            (A008281): E(0,0)=1, E(m,0)=0 for m>=1,
 *            E(m,k)=E(m,k-1)+E(m-1,m-k) for 1<=k<=m
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 15, i64 n <= 23, i128 n <= 24,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 24 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *A005437_grow_zero(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    if (p == NULL) return NULL;
    mem_set(p, 0, (size_t)newlen * sizeof(seq_t));
    if (old != NULL) {
        mem_copy(p, old, (size_t)oldlen * sizeof(seq_t));
        mem_free(old);
    }
    return p;
}

static seq_t *A005437_P = NULL;
static seq_t *A005437_Q = NULL;
static seq_t *A005437_R = NULL;
static idx_t A005437_cap = 0;
static idx_t A005437_rows = 0;

static seq_t A005437_a(idx_t n) {
    if (n < A005437_rows) return A005437_R[n];

    if (n + 1 > A005437_cap) {
        idx_t newcap = 2 * A005437_cap;
        if (newcap < n + 1) newcap = n + 1;
        A005437_P = A005437_grow_zero(A005437_P, A005437_cap, newcap);
        A005437_Q = A005437_grow_zero(A005437_Q, A005437_cap, newcap);
        A005437_R = A005437_grow_zero(A005437_R, A005437_cap, newcap);
        A005437_cap = newcap;
    }

    if (A005437_rows == 0) {
        A005437_P[0] = 1;
        A005437_R[0] = 1;
        A005437_rows = 1;
    }

    while (A005437_rows <= n) {
        idx_t m = A005437_rows;
        seq_t *t;
        idx_t k;
        A005437_Q[0] = 0;
        for (k = 1; k <= m; k++)
            A005437_Q[k] = A005437_Q[k - 1] + A005437_P[m - k];
        A005437_R[m] = A005437_Q[(m + 1) / 2];
        t = A005437_P; A005437_P = A005437_Q; A005437_Q = t;
        A005437_rows++;
    }

    return A005437_R[n];
}

seq_t a(idx_t n) {
    return A005437_a(n);
}
