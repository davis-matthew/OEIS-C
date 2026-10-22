/*
 * A003228  Endpoints in trees with n nodes.
 * https://oeis.org/A003228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = q(n) + p(n-1) + [n even]*q(n/2) - Sum_{i=1..n-1}
 *            p(i)*q(n-i), where p = A000081 (rooted trees), q(n) = [n=1] +
 *            Sum_{t=1..n-1} p(t)*s(n-t), s(m) = Sum_{d|m} q(d) (q(n) = total
 *            childless nodes over rooted trees with n nodes); this is d/dy at
 *            y=1 of (1-x+x*y)*B(x,y) + (B(x^2,y^2) - B(x,y)^2)/...
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   A_grow, a3228_gcdi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/A_grow.h"
#include "../utils/a3228_gcdi.h"

static seq_t *Pp = NULL, *Qq = NULL, *Ss = NULL, *Cc = NULL;
static idx_t cap = 0;
static idx_t have = 0;

static void a3228_build_q(idx_t from, idx_t N)
{
    for (idx_t m = from; m <= N; m++) {
        seq_t q = (m == 1) ? (seq_t)1 : (seq_t)0;
        for (idx_t t = 1; t < m; t++) q += Pp[t] * Ss[m - t];
        Qq[m] = q;
        seq_t s = 0;
        for (idx_t d = 1; d * d <= m; d++) {
            if (m % d == 0) {
                s += Qq[d];
                if (d * d != m) s += Qq[m / d];
            }
        }
        Ss[m] = s;
    }
}

/* rooted trees via forest DP with exact multichoose (no wrapping division) */
static void a3228_fixed_p(idx_t N)
{
    seq_t *F = (seq_t *)malloc((size_t)(N + 1) * sizeof(seq_t));
    seq_t *mc = (seq_t *)malloc((size_t)(N + 1) * sizeof(seq_t));
    for (idx_t i = 0; i <= N; i++) F[i] = 0;
    F[0] = 1;
    for (idx_t k = 1; k <= N; k++) {
        seq_t p = F[k - 1];
        Pp[k] = p;
        if (k + 1 > N) continue;
        idx_t top = N - 1;            /* forest sizes needed: 0..N-1 */
        if (top < k) continue;
        idx_t J = top / k;
        mc[0] = 1;
        for (idx_t j = 1; j <= J; j++) {
            seq_t r = mc[j - 1];
            idx_t rm = (idx_t)(r % j);
            idx_t g = a3228_gcdi(j, rm);
            mc[j] = (r / g) * ((p + (seq_t)(j - 1)) / (seq_t)(j / g));
        }
        for (idx_t m = top; m >= k; m--) {
            seq_t acc = 0;
            for (idx_t j = 1; j * k <= m; j++) acc += mc[j] * F[m - j * k];
            F[m] += acc;
        }
    }
    free(F);
    free(mc);
}

static void a3228_ensure(idx_t N, int fixed)
{
    if (N <= have) return;
    if (N + 1 > cap) {
        idx_t nc = cap * 2;
        if (nc < N + 1) nc = N + 1;
        Pp = A_grow(Pp, cap, nc);
        Qq = A_grow(Qq, cap, nc);
        Ss = A_grow(Ss, cap, nc);
        Cc = A_grow(Cc, cap, nc);
        cap = nc;
    }
    if (fixed) {
        a3228_fixed_p(N);
        a3228_build_q(1, N);
        have = N;
        return;
    }
    for (idx_t m = have + 1; m <= N; m++) {
        if (m == 1) {
            Pp[1] = 1;
        } else {
            idx_t j = m - 1;
            seq_t c = 0;
            for (idx_t d = 1; d * d <= j; d++) {
                if (j % d == 0) {
                    c += Pp[d] * (seq_t)d;
                    if (d * d != j) c += Pp[j / d] * (seq_t)(j / d);
                }
            }
            Cc[j] = c;
            seq_t sum = 0;
            for (idx_t i = 1; i <= j; i++) sum += Cc[i] * Pp[m - i];
            Pp[m] = sum / (seq_t)j;
        }
        a3228_build_q(m, m);
        have = m;
    }
}

seq_t a(idx_t n)
{
    int fixed = ((seq_t)0 - 1) > 0;
    if (n < 1) return 0;
    if (fixed && n > 1000) return 0;   /* far beyond any fixed-width range */
    a3228_ensure(n, fixed);
    seq_t pos = Qq[n];
    if (n >= 2) pos += Pp[n - 1];
    if (n % 2 == 0) pos += Qq[n / 2];
    seq_t neg = 0;
    for (idx_t i = 1; i < n; i++) neg += Pp[i] * Qq[n - i];
    return pos - neg;
}
