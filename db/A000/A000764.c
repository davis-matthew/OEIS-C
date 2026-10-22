/*
 * A000764  Boustrophedon transform of Bell numbers.
 * https://oeis.org/A000764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(n,n) for the boustrophedon triangle T(k,0) = B(k) (Bell
 *            numbers, from the Bell triangle), T(k,j) = T(k,j-1) +
 *            T(k-1,k-j); equivalently a(n) = Sum_{k=0..n}
 *            binomial(n,k)*Bell(k)*A000111(n-k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 37,
 *            big any n
 * Helpers:   grow_copy
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_copy.h"

static seq_t *A_ = 0;   /* computed terms */
static seq_t *BR = 0;   /* current Bell-triangle row */
static seq_t *TR = 0;   /* current boustrophedon row */
static seq_t *TMP = 0;  /* scratch row */
static idx_t done = 0;  /* number of terms computed (rows 0..done-1) */
static idx_t cap = 0;

static void ensure_cap(idx_t need) {
    idx_t nc;
    if (need <= cap) return;
    nc = cap * 2;
    if (nc < need) nc = need;
    if (nc < 16) nc = 16;
    A_ = grow_copy(A_, done, nc);
    BR = grow_copy(BR, done, nc);
    TR = grow_copy(TR, done, nc);
    TMP = grow_copy(TMP, 0, nc);
    cap = nc;
}

seq_t a(idx_t n) {
    idx_t k, j;
    seq_t *t;
    ensure_cap(n + 2);
    if (done == 0) {
        BR[0] = 1;
        TR[0] = 1;
        A_[0] = 1;
        done = 1;
    }
    while (done <= n) {
        k = done;
        /* Bell triangle row k from row k-1 (length k) */
        TMP[0] = BR[k - 1];
        for (j = 1; j <= k; j++) TMP[j] = TMP[j - 1] + BR[j - 1];
        t = BR; BR = TMP; TMP = t;
        /* boustrophedon row k from row k-1 (length k) */
        TMP[0] = BR[0];
        for (j = 1; j <= k; j++) TMP[j] = TMP[j - 1] + TR[k - j];
        t = TR; TR = TMP; TMP = t;
        A_[k] = TR[k];
        done = k + 1;
    }
    return A_[n];
}
