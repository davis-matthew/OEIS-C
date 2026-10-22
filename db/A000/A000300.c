/*
 * A000300  4th power of rooted tree enumerator: linear forests of 4 rooted
 *          trees.
 * https://oeis.org/A000300
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=2..n-2} C(j)*C(n-j), where C = B^2, i.e. C(j) =
 *            Sum_{i=1..j-1} t(i)*t(j-i), and B(x) = Sum t(k) x^k is the g.f.
 *            of A000081: t(1) = 1, t(m+1) = (1/m) * Sum_{k=1..m}
 *            s(k)*t(m-k+1), s(k) = Sum_{d|k} d*t(d). G.f.: B(x)^4
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 24, u64 n <= 45, u128 n <= 87,
 *            big any n
 * Helpers:   zalloc_s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/zalloc_s.h"

static seq_t *T = 0;   /* A000081: T[1..have] */
static seq_t *S = 0;   /* S[k] = Sum_{d|k} d*T[d], k = 1..have */
static seq_t *B2 = 0;  /* coefficients of B(x)^2, indices 0..have+1 */
static idx_t have = 0;
static idx_t cap = 0;

static void grow(idx_t need)
{
    idx_t nc, i;
    seq_t *nt, *ns, *nb;
    if (need < cap) return;
    nc = cap ? cap : 64;
    while (nc <= need) nc *= 2;
    nt = zalloc_s(nc);
    ns = zalloc_s(nc);
    nb = zalloc_s(nc);
    if (have > 0) {
        for (i = 0; i <= have + 1; i++) {
            nt[i] = T[i];
            ns[i] = S[i];
            nb[i] = B2[i];
        }
    }
    T = nt;
    S = ns;
    B2 = nb;
    cap = nc;
}

/* make T, S known up to N and B2 up to N+1 */
static void extend(idx_t N)
{
    grow(N + 2);
    if (have == 0) {
        T[0] = 0; S[0] = 0; B2[0] = 0;
        T[1] = 1; S[1] = 1; B2[1] = 0;
        B2[2] = 1;
        have = 1;
    }
    while (have < N) {
        idx_t m = have, j = m + 1, k, d, i;
        seq_t acc = 0, sd = 0, c = 0;
        for (k = 1; k <= m; k++)
            acc += S[k] * T[m - k + 1];
        T[j] = acc / m;
        for (d = 1; d * d <= j; d++) {
            if (j % d == 0) {
                idx_t e = j / d;
                sd += (seq_t)d * T[d];
                if (e != d) sd += (seq_t)e * T[e];
            }
        }
        S[j] = sd;
        for (i = 1; i <= j; i++)
            c += T[i] * T[j + 1 - i];
        B2[j + 1] = c;
        have = j;
    }
}

seq_t a(idx_t n)
{
    seq_t r = 0;
    idx_t j;
    if (n < 4) return 0;
    extend(n - 3);
    for (j = 2; j <= n - 2; j++)
        r += B2[j] * B2[n - j];
    return r;
}
