/*
 * A007116  Expansion of e.g.f. (1+x)^(exp(x)).
 * https://oeis.org/A007116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   E.g.f. f(x) = (1+x)^exp(x) = exp(g(x)), g(x) = exp(x)*log(1+x),
 *            so f' = g'*f: a(n+1) = Sum_{k=0..n} binomial(n,k)*b(k+1)*a(n-k),
 *            a(0) = 1, where b(m) = m![x^m] g(x): b(0) = 0, b(m+1) = b(m) +
 *            d(m), and d(m) = m![x^m] exp(x)/(1+x): d(0) = 1, d(m) = 1 -
 *            m*d(m-1)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 14, i64 n <= 22, i128 n <= 35,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

static seq_t *A_memo = NULL;   /* a(0..A_len-1) */
static idx_t A_len = 0;
static seq_t *B_memo = NULL;   /* b(0..B_len-1) */
static seq_t *D_memo = NULL;   /* d(0..B_len-1) */
static idx_t B_len = 0;
static idx_t B_cap = 0;
static idx_t A_cap = 0;

static void ensure_b(idx_t len) {
    if (B_len >= len) return;
    if (len > B_cap) {
        idx_t nc = B_cap ? B_cap : 16;
        while (nc < len) nc *= 2;
        seq_t *nb = (seq_t *)malloc(nc * sizeof(seq_t));
        seq_t *nd = (seq_t *)malloc(nc * sizeof(seq_t));
        for (idx_t i = 0; i < B_len; i++) { nb[i] = B_memo[i]; nd[i] = D_memo[i]; }
        for (idx_t i = B_len; i < nc; i++) { nb[i] = 0; nd[i] = 0; }
        free(B_memo); free(D_memo);
        B_memo = nb; D_memo = nd; B_cap = nc;
    }
    if (B_len == 0) {
        B_memo[0] = 0;
        D_memo[0] = 1;
        B_len = 1;
    }
    while (B_len < len) {
        idx_t m = B_len;
        B_memo[m] = B_memo[m - 1] + D_memo[m - 1];
        D_memo[m] = (seq_t)1 - (seq_t)m * D_memo[m - 1];
        B_len++;
    }
}

static void ensure_a(idx_t len) {
    if (A_len >= len) return;
    if (len > A_cap) {
        idx_t nc = A_cap ? A_cap : 16;
        while (nc < len) nc *= 2;
        seq_t *na = (seq_t *)malloc(nc * sizeof(seq_t));
        for (idx_t i = 0; i < A_len; i++) na[i] = A_memo[i];
        for (idx_t i = A_len; i < nc; i++) na[i] = 0;
        free(A_memo);
        A_memo = na; A_cap = nc;
    }
    ensure_b(len + 1);
    if (A_len == 0) {
        A_memo[0] = 1;
        A_len = 1;
    }
    while (A_len < len) {
        idx_t n = A_len - 1;   /* computing a(n+1) */
        seq_t s = 0;
        for (idx_t k = 0; k <= n; k++) {
            seq_t t = B_memo[k + 1];
            if (t != 0) s += binom(n, k) * t * A_memo[n - k];
        }
        A_memo[n + 1] = s;
        A_len++;
    }
}

seq_t a(idx_t n) {
    ensure_a(n + 1);
    return A_memo[n];
}
