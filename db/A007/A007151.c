/*
 * A007151  Number of planted evolutionary trees of magnitude n.
 * https://oeis.org/A007151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   E.g.f. A(x) satisfies (2-x)*A(x) = x - 1 + exp(A(x)). With b(n)
 *            = n![x^n] exp(A(x)) and exp(A)' = A'*exp(A): a(n) = n*a(n-1) +
 *            Sum_{k=0..n-2} binomial(n-1,k)*a(k+1)*b(n-1-k), b(n) = 2*a(n) -
 *            n*a(n-1) for n >= 2, a(0) = 0, a(1) = 1, b(0) = 1, b(1) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/grow_zero.h"

static seq_t *A_ = 0;    /* a(i) */
static seq_t *B_ = 0;    /* b(i) */
static seq_t *ROW = 0;   /* binomial(done-1, k) */
static idx_t cap = 0;
static idx_t done = 0;

static void ensure_cap(idx_t need) {
    if (need <= cap) return;
    idx_t nc = cap * 2;
    if (nc < need) nc = need;
    if (nc < 16) nc = 16;
    A_ = grow_zero(A_, cap, nc);
    B_ = grow_zero(B_, cap, nc);
    ROW = grow_zero(ROW, cap, nc);
    cap = nc;
}

seq_t a(idx_t n) {
    if (done == 0) {
        ensure_cap(16);
        A_[0] = 0; A_[1] = 1;
        B_[0] = 1; B_[1] = 1;
        ROW[0] = 1;
        done = 1;
    }
    if (n <= done) return A_[n];
    ensure_cap(n + 1);
    for (idx_t m = done + 1; m <= n; m++) {
        /* update ROW from binomial(m-2, .) to binomial(m-1, .) */
        for (idx_t k = m - 1; k >= 1; k--) {
            ROW[k] = ROW[k] + ROW[k - 1];
        }
        ROW[0] = 1;
        seq_t s = 0;
        for (idx_t k = 0; k + 2 <= m; k++) {
            s += ROW[k] * A_[k + 1] * B_[m - 1 - k];
        }
        seq_t prev = (seq_t)m * A_[m - 1];
        A_[m] = s + prev;
        B_[m] = A_[m] + A_[m] - prev;
        done = m;
    }
    return A_[n];
}
