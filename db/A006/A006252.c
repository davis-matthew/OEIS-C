/*
 * A006252  Expansion of e.g.f. 1/(1 - log(1+x)).
 * https://oeis.org/A006252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(k-1) * (k-1)! * binomial(n,k) *
 *            a(n-k), a(0) = 1
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 14, i64 n <= 22, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> memo_v, row_v;
static void ensure_cap(size_t len) {
    if (memo_v.size() < len) {
        memo_v.resize(len);
        row_v.resize(len);
    }
}
#define MEMO (memo_v.data())
#define ROW (row_v.data())
#else
static seq_t *memo_p = NULL;
static seq_t *row_p = NULL;
static size_t cap_p = 0;
static void ensure_cap(size_t len) {
    if (cap_p < len) {
        size_t nc = len * 2;
        size_t i;
        memo_p = (seq_t *)realloc(memo_p, nc * sizeof(seq_t));
        row_p = (seq_t *)realloc(row_p, nc * sizeof(seq_t));
        for (i = cap_p; i < nc; i++) {
            memo_p[i] = 0;
            row_p[i] = 0;
        }
        cap_p = nc;
    }
}
#define MEMO memo_p
#define ROW row_p
#endif

/* number of terms computed so far; ROW holds binomial row (done-1) */
static idx_t done = 0;

seq_t a(idx_t n) {
    idx_t i, k;
    if (done == 0) {
        ensure_cap(16);
        MEMO[0] = 1;
        ROW[0] = 1;
        done = 1;
    }
    if (n < done) return MEMO[n];
    ensure_cap((size_t)n + 1);
    for (i = done; i <= n; i++) {
        seq_t *memo = MEMO;
        seq_t *row = ROW;
        seq_t s = 0;
        seq_t f = 1; /* (k-1)! */
        /* advance Pascal row from i-1 to i (ring operations only) */
        row[i] = 1;
        for (k = i - 1; k >= 1; k--) {
            row[k] += row[k - 1];
        }
        for (k = 1; k <= i; k++) {
            seq_t t = f * row[k];
            t = t * memo[i - k];
            if (k & 1) s += t;
            else s -= t;
            f *= (seq_t)k;
        }
        memo[i] = s;
        done = i + 1;
    }
    return MEMO[n];
}
