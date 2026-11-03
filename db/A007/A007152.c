/*
 * A007152  Evolutionary trees of magnitude n.
 * https://oeis.org/A007152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = c(n-1) + (1/2)*Sum_{i=1..n-2}
 *            binomial(n-1,i)*c(i)*c(n-1-i) for n >= 2, where B(x) = Sum c(m)
 *            x^m/m! satisfies (2-x)*B = x + exp(B) - 1, i.e. c(m) = m*c(m-1)
 *            + [m=1] + R(m), e(m) = c(m) + R(m), R(m) = Sum_{i=1..m-1}
 *            binomial(m-1,i-1)*c(i)*e(m-i), c(0) = 0, e(0) = 1. (Trees
 *            whose...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *grow_zero(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    if (p == NULL) return NULL;
    mem_set(p, 0, (size_t)newlen * sizeof(seq_t));
    if (old != NULL) {
        mem_copy(p, old, (size_t)oldlen * sizeof(seq_t));
        mem_free(old);
    }
    return p;
}

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

static seq_t *Cc = NULL, *Ee = NULL, *Pp = NULL;
static idx_t cap = 0;
static idx_t cnt = 0; /* c,e known for m < cnt; Pp holds Pascal row cnt-1 */

static void extend(idx_t M)
{
    idx_t m, i;
    if (M + 2 > cap) {
        idx_t nc = (M + 2) * 2;
        Cc = grow_zero(Cc, cap, nc);
        Ee = grow_zero(Ee, cap, nc);
        Pp = grow_zero(Pp, cap, nc);
        cap = nc;
    }
    if (cnt == 0) {
        Cc[0] = 0;
        Ee[0] = 1;
        Pp[0] = 1;
        cnt = 1;
    }
    while (cnt <= M) {
        seq_t R = 0;
        m = cnt;
        for (i = 1; i < m; i++)
            R += Pp[i - 1] * Cc[i] * Ee[m - i];
        Cc[m] = (seq_t)m * Cc[m - 1] + R;
        if (m == 1) Cc[m] += 1;
        Ee[m] = Cc[m] + R;
        for (i = m; i >= 1; i--)
            Pp[i] += Pp[i - 1];
        cnt++;
    }
}

seq_t a(idx_t n)
{
    idx_t N, r, i;
    seq_t S, mid, res;
    seq_t *Q;
    if (n <= 1) return 1;
    N = n - 1;
    extend(N);
    Q = alloc_zero(N + 2);
    Q[0] = 1;
    for (r = 1; r <= N - 1; r++)
        for (i = r; i >= 1; i--)
            Q[i] += Q[i - 1];
    mid = 0;
    if (N % 2 == 0)
        mid = Q[N / 2 - 1] * Cc[N / 2] * Cc[N / 2];
    for (i = N; i >= 1; i--)
        Q[i] += Q[i - 1];
    S = 0;
    for (i = 1; 2 * i < N; i++)
        S += Q[i] * Cc[i] * Cc[N - i];
    res = Cc[N] + S + mid;
    release(Q, N + 2);
    return res;
}
