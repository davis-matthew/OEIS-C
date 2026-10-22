/*
 * A004704  Expansion of e.g.f. 1/(7- Sum_{k=1..6} exp(k*x)).
 * https://oeis.org/A004704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k) * (1^k + 2^k + ... + 6^k) *
 *            a(n-k), a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *A = NULL;    /* A[0..len-1] computed terms */
static seq_t *row = NULL;  /* Pascal row len-1: row[0..len-1] */
static seq_t *S = NULL;    /* S[k] = 1^k + ... + 6^k */
static seq_t pw[7];
static idx_t len = 0, cap = 0;

static seq_t *grow_copy(seq_t *old, idx_t oldlen, idx_t newcap) {
    seq_t *p = (seq_t *)malloc((size_t)newcap * sizeof(seq_t));
    idx_t i;
    for (i = 0; i < newcap; i++) p[i] = 0;
    for (i = 0; i < oldlen; i++) p[i] = old[i];
    if (old) free(old);
    return p;
}

static void ensure_cap(idx_t need) {
    idx_t nc;
    if (need <= cap) return;
    nc = cap ? cap : 16;
    while (nc < need) nc *= 2;
    A = grow_copy(A, len, nc);
    row = grow_copy(row, len, nc);
    S = grow_copy(S, len, nc);
    cap = nc;
}

seq_t a(idx_t n) {
    idx_t m, j, k;
    int i;
    if (len == 0) {
        ensure_cap(16);
        A[0] = 1;
        row[0] = 1;
        S[0] = 6;
        for (i = 0; i <= 6; i++) pw[i] = 1;
        len = 1;
    }
    if (n < len) return A[n];
    ensure_cap(n + 1);
    for (m = len; m <= n; m++) {
        seq_t s, r;
        /* advance Pascal row from m-1 to m */
        row[m] = 1;
        for (j = m - 1; j >= 1; j--) row[j] += row[j - 1];
        /* power sum for exponent m */
        s = 0;
        for (i = 1; i <= 6; i++) {
            pw[i] *= (seq_t)i;
            s += pw[i];
        }
        S[m] = s;
        r = 0;
        for (k = 1; k <= m; k++) r += row[k] * S[k] * A[m - k];
        A[m] = r;
        len = m + 1;
    }
    return A[n];
}
