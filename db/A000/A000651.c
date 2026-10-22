/*
 * A000651  Running time of Takeuchi function.
 * https://oeis.org/A000651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   from the g.f. A(z-z^2)/z - A(z) = 1/(1-z) + z/(1-z+z^2): a(n+1)
 *            = r(n) + a(n) - Sum_{k=0..n} (-1)^(n+1-k) * binomial(k, n+1-k) *
 *            a(k), with r(n) = 1 + [z^n] z/(1-z+z^2), a(0) = 0, a(1) = 1.
 *            Each antidiagonal D_s[k] = binomial(k, s-k) of Pascal's triangle
 *            is built from the two before it: D_s[k] =...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 41,
 *            big any n
 * Helpers:   a306_alloc0, a306_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/a306_alloc0.h"
#include "../utils/a306_grow.h"

static seq_t *A = NULL;   /* A[0..m] computed */
static seq_t *Dc = NULL;  /* D_m */
static seq_t *Dp = NULL;  /* D_{m-1} */
static idx_t cap = 0;
static idx_t m = 0;

static void ensure_cap(idx_t need) {
    idx_t nc;
    if (need <= cap) return;
    nc = cap * 2;
    if (nc < need) nc = need;
    if (nc < 64) nc = 64;
    if (A == NULL) {
        A = a306_alloc0(nc);
        Dc = a306_alloc0(nc);
        Dp = a306_alloc0(nc);
    } else {
        A = a306_grow(A, cap, nc);
        Dc = a306_grow(Dc, cap, nc);
        Dp = a306_grow(Dp, cap, nc);
    }
    cap = nc;
}

seq_t a(idx_t n) {
    static const int rtab[6] = {2, 2, 1, 0, 0, 1}; /* r(m) for (m-1) mod 6 */
    if (A == NULL) {
        ensure_cap(64);
        A[0] = 0;
        A[1] = 1;
        Dp[0] = 1;          /* D_0 = [1] */
        Dc[0] = 0;          /* D_1 = [0, 1] */
        Dc[1] = 1;
        m = 1;
    }
    if (n <= m) return A[n];
    ensure_cap(n + 2);
    while (m < n) {
        idx_t k;
        seq_t *t;
        seq_t pos, neg;
        /* D_{m+1}[k] = D_m[k-1] + D_{m-1}[k-1], built in place over Dp */
        for (k = m + 1; k >= 1; k--)
            Dp[k] = Dc[k - 1] + Dp[k - 1];
        Dp[0] = 0;
        t = Dp; Dp = Dc; Dc = t;   /* Dc = D_{m+1}, Dp = D_m */

        pos = A[m];
        pos += (seq_t)rtab[(m - 1) % 6];
        neg = 0;
        for (k = (m + 1) / 2; k <= m; k++) {
            idx_t j = m + 1 - k;
            if (j & 1)
                pos += Dc[k] * A[k];
            else
                neg += Dc[k] * A[k];
        }
        A[m + 1] = pos - neg;
        m++;
    }
    return A[n];
}
