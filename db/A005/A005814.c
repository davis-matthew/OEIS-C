/*
 * A005814  Number of 3-regular (trivalent) labeled graphs on 2n vertices with
 *          multiple edges and loops allowed.
 * https://oeis.org/A005814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   with b(m) = a(m)/(2m)!, the e.g.f. ODE of V. Jovovic gives
 *            48*(m+1)*a(m+1) = (2m+2)(2m+1) * [ c0*a(m) + (2m)(2m-1)*[
 *            c1*a(m-1) + (2m-2)(2m-3)*[ c2*a(m-2) + (2m-4)(2m-5)*[ c3*a(m-3)
 *            + (2m-6)(2m-7)*[ c4*a(m-4) + (2m-8)(2m-9)*c5*a(m-5) ]]]]] with
 *            c0 = 72m^2+24m+48, c1 = 72m^2-72m+44, c2 = -(36m^2-21...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 93 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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

static bool seq_is_fixed_width(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0)
            return true;
    }
    return false;
}

static const int PJ[6] = {1, 2, 2, 3, 3, 3};
static const int PE[6] = {1, 1, 2, 1, 2, 3};

static seq_t *dp_val = NULL;
static unsigned char *dp_done = NULL;
static long long dp_N = 0;

static seq_t dp_f(long long c1, long long c2, long long c3);

static seq_t dp_dist(long long *av, long long *ad, int r, int p)
{
    if (r == 0)
        return dp_f(av[1] + ad[1], av[2] + ad[2], av[3] + ad[3]);
    if (p == 6)
        return 0;
    seq_t s = dp_dist(av, ad, r, p + 1);
    int j = PJ[p], e = PE[p];
    long long A = av[j];
    long long coef = 1;
    long long k;
    for (k = 1; k * e <= r && k <= A; k++) {
        coef = coef * (A - k + 1) / k;
        av[j] = A - k;
        if (j - e > 0) ad[j - e] += k;
        s += dp_dist(av, ad, r - (int)(k * e), p + 1) * (idx_t)coef;
        if (j - e > 0) ad[j - e] -= k;
        av[j] = A;
    }
    return s;
}

static seq_t dp_f(long long c1, long long c2, long long c3)
{
    if (c1 == 0 && c2 == 0 && c3 == 0)
        return 1;
    long long W = dp_N + 1;
    long long idx = (c1 * W + c2) * W + c3;
    if (dp_done[idx])
        return dp_val[idx];
    int d = c1 ? 1 : (c2 ? 2 : 3);
    long long av[4], ad[4];
    av[0] = 0; av[1] = c1; av[2] = c2; av[3] = c3;
    ad[0] = ad[1] = ad[2] = ad[3] = 0;
    av[d]--;
    seq_t s = 0;
    int l;
    for (l = 0; 2 * l <= d; l++)
        s += dp_dist(av, ad, d - 2 * l, 0);
    dp_val[idx] = s;
    dp_done[idx] = 1;
    return s;
}

seq_t a(idx_t n)
{
    if (seq_is_fixed_width()) {
        if (n > 20)
            return 0; /* overflows every fixed-width type */
        long long N = 2 * (long long)n;
        idx_t sz = (idx_t)((N + 1) * (N + 1) * (N + 1));
        dp_N = N;
        dp_val = alloc_zero(sz);
        dp_done = (unsigned char *)calloc((size_t)sz, 1);
        seq_t r = dp_f(0, 0, N);
        release(dp_val, sz);
        free(dp_done);
        dp_val = NULL;
        dp_done = NULL;
        return r;
    }

    static seq_t *memo = NULL;
    static idx_t cnt = 0, cap = 0;
    if (cnt == 0) {
        cap = 16;
        memo = grow_zero(memo, 0, cap);
        memo[0] = 1;
        cnt = 1;
    }
    while (cnt <= n) {
        if (cnt >= cap) {
            idx_t nc = cap * 2;
            memo = grow_zero(memo, cap, nc);
            cap = nc;
        }
        idx_t m = cnt - 1;
        long long mm = (long long)m;
        long long c[6];
        c[0] = 72 * mm * mm + 24 * mm + 48;
        c[1] = 72 * mm * mm - 72 * mm + 44;
        c[2] = -(36 * mm * mm - 216 * mm + 284);
        c[3] = -(36 * mm - 84);
        c[4] = 6 * mm - 14;
        c[5] = -1;
        int kmax = m < 5 ? (int)m : 5;
        seq_t P = 0, Q = 0;
        int k;
        for (k = kmax; k >= 0; k--) {
            if (k < kmax) {
                idx_t g = (2 * m - 2 * (idx_t)k) * (2 * m - 1 - 2 * (idx_t)k);
                P *= g;
                Q *= g;
            }
            if (c[k] > 0)
                P += memo[m - (idx_t)k] * (idx_t)c[k];
            else if (c[k] < 0)
                Q += memo[m - (idx_t)k] * (idx_t)(-c[k]);
        }
        seq_t S = P - Q;
        S *= (idx_t)((2 * m + 2) * (2 * m + 1));
        S /= (idx_t)(48 * (m + 1));
        memo[cnt] = S;
        cnt++;
    }
    return memo[n];
}
