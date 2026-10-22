/*
 * A006714  Number of trivalent bipartite labeled graphs with 2n labeled
 *          nodes.
 * https://oeis.org/A006714
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   with R(n) = A001501(n) = 36^(-n) * Sum_{alpha+beta+gamma=n}
 *            (-1)^beta * n!^2 * (beta+3*gamma)! * 12^alpha * 18^beta /
 *            (alpha!*beta!*gamma!^2) (number of 2-colored labeled graphs =
 *            binomial(2n,n)*R(n) = Sum_{k=0..n} binomial(2n,2k)*a(k)*a(n-k),
 *            a(0)=1), i.e. a(n) = binomial(2n-1,n)*(R(n) - [n even...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, release, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_big.h"
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"

static seq_t mybinom(idx_t m, idx_t k)
{
    idx_t i;
    if (k > m) return 0;
    if (seq_is_big()) {
        seq_t r = 1;
        for (i = 0; i < k; i++) r = r * (seq_t)(m - i) / (seq_t)(i + 1);
        return r;
    } else {
        uint64_t r = 1;
        for (i = 0; i < k; i++) r = r * (uint64_t)(m - i) / (uint64_t)(i + 1);
        return (seq_t)r;
    }
}

static uint64_t c3(long long x, int i)
{
    if (i == 0) return 1;
    if (i == 1) return (uint64_t)x;
    if (i == 2) return (uint64_t)(x * (x - 1) / 2);
    return (uint64_t)(x * (x - 1) * (x - 2) / 6);
}

/* number of n x n 0-1 matrices with all row/column sums 3, by column DP (ring ops only) */
static seq_t r_dp(idx_t n)
{
    idx_t W = n + 1, c, x3, x2;
    seq_t *cur = alloc_zero(W * W), *nxt = alloc_zero(W * W), *tmp;
    seq_t res;
    cur[n * W + 0] = 1;
    for (c = n; c > 0; c--) {
        idx_t q;
        for (q = 0; q < W * W; q++) nxt[q] = 0;
        for (x3 = 0; x3 <= n; x3++)
            for (x2 = 0; x2 <= n; x2++) {
                seq_t v = cur[x3 * W + x2];
                long long x1;
                int i, j, k;
                if (v == 0) continue;
                x1 = 3LL * (long long)c - 3LL * (long long)x3 - 2LL * (long long)x2;
                if (x1 < 0) continue;
                for (i = 0; i <= 3; i++)
                    for (j = 0; i + j <= 3; j++) {
                        uint64_t coef;
                        idx_t n3, n2;
                        k = 3 - i - j;
                        if ((idx_t)i > x3 || (idx_t)j > x2 || (long long)k > x1) continue;
                        coef = c3((long long)x3, i) * c3((long long)x2, j) * c3(x1, k);
                        n3 = x3 - (idx_t)i;
                        n2 = x2 - (idx_t)j + (idx_t)i;
                        if (n2 > n) continue;
                        nxt[n3 * W + n2] += v * (seq_t)coef;
                    }
            }
        tmp = cur; cur = nxt; nxt = tmp;
    }
    res = cur[0];
    release(cur, W * W);
    release(nxt, W * W);
    return res;
}

/* same count by Read's formula, exact big-integer arithmetic */
static seq_t r_big(idx_t n)
{
    idx_t F = 3 * n + 1, i, g, b;
    seq_t *f = alloc_zero(F);
    seq_t *p12 = alloc_zero(n + 1);
    seq_t P = 0, N = 0, p36 = 1, res;
    f[0] = 1;
    for (i = 1; i < F; i++) f[i] = f[i - 1] * (seq_t)i;
    p12[0] = 1;
    for (i = 1; i <= n; i++) p12[i] = p12[i - 1] * 12;
    for (i = 0; i < n; i++) p36 *= 36;
    for (g = 0; g <= n; g++) {
        idx_t m = n - g;
        seq_t Pg = 0, Ng = 0, t = p12[m], q, K;
        for (b = 0; b <= m; b++) {
            seq_t term = t * f[b + 3 * g];
            if (b % 2 == 0) Pg += term; else Ng += term;
            if (b < m) t = t * (seq_t)(3 * (m - b)) / (seq_t)(2 * (b + 1));
        }
        q = f[n] / f[g];
        K = q * (q / f[m]);
        P += K * Pg;
        N += K * Ng;
    }
    res = (P - N) / p36;
    release(f, F);
    release(p12, n + 1);
    return res;
}

seq_t a(idx_t n)
{
    static seq_t *A = NULL;
    static idx_t len = 0;
    if (!seq_is_big() && n > 100) return 0; /* far beyond any fixed width */
    if (n >= len) {
        idx_t nl = n + 1, m, k;
        A = grow_zero(A, len, nl);
        for (m = len; m < nl; m++) {
            seq_t R, c, total, sub;
            if (m == 0) { A[0] = 1; continue; }
            R = seq_is_big() ? r_big(m) : r_dp(m);
            c = mybinom(2 * m - 1, m);
            total = c * R;
            sub = 0;
            if (m % 2 == 0) sub += c * A[m / 2] * A[m / 2];
            for (k = 1; 2 * k < m; k++)
                if (A[k] != 0 && A[m - k] != 0)
                    sub += mybinom(2 * m, 2 * k) * A[k] * A[m - k];
            A[m] = total - sub;
        }
        len = nl;
    }
    return A[n];
}
