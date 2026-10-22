/*
 * A004706  Expansion of e.g.f. 1/(9 - Sum_{k=1..8} exp(k*x)).
 * https://oeis.org/A004706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=1..n} binomial(n,k) * (1 + 2^k + ... +
 *            8^k) * a(n-k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *A = NULL;   /* computed terms a(0..cnt-1) */
static seq_t *S = NULL;   /* S[k] = 1^k + 2^k + ... + 8^k */
static seq_t *row = NULL; /* Pascal row for index cnt-1 */
static seq_t pw[9];       /* pw[j] = j^(cnt-1) */
static idx_t cnt = 0, cap = 0;

static seq_t *grow_copy(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = (seq_t *)malloc((size_t)newlen * sizeof(seq_t));
    idx_t i;
    for (i = 0; i < newlen; i++) p[i] = 0;
    for (i = 0; i < oldlen; i++) p[i] = old[i];
    if (old) free(old);
    return p;
}

seq_t a(idx_t n) {
    idx_t i, k;
    int j;
    if (n >= cap) {
        idx_t nc = cap ? cap : 64;
        while (nc <= n) nc *= 2;
        A = grow_copy(A, cap, nc);
        S = grow_copy(S, cap, nc);
        row = grow_copy(row, cap, nc);
        cap = nc;
    }
    for (i = cnt; i <= n; i++) {
        if (i == 0) {
            A[0] = 1;
            row[0] = 1;
            S[0] = 8;
            for (j = 1; j <= 8; j++) pw[j] = 1;
        } else {
            seq_t s = 0, sum = 0;
            for (k = i; k >= 1; k--) row[k] = row[k] + row[k - 1];
            for (j = 1; j <= 8; j++) {
                pw[j] = pw[j] * (seq_t)j;
                s = s + pw[j];
            }
            S[i] = s;
            for (k = 1; k <= i; k++) sum = sum + row[k] * S[k] * A[i - k];
            A[i] = sum;
        }
        cnt = i + 1;
    }
    return A[n];
}
