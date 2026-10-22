/*
 * A000886  Number of switching networks with S(n,2) acting on the domain and
 *          AG(2,2) acting on the range where S(n,k) is the symmetric group
 *          acting on k variables.
 * https://oeis.org/A000886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside count of orbits of maps f:{0,1}^n -> {0,1}^2 under S_n
 *            (permuting variables) x AG(2,2) (= S_4 on {0,1}^2): a(n) =
 *            (1/(24 n!)) * Sum_{lambda |- n} (n!/z_lambda) * Sum_{tau in S_4}
 *            Prod_L fix(tau^L)^{N_L(lambda)}, where N_L(lambda) = (1/L)
 *            Sum_{d|L} mu(L/d) 2^{Sum_i gcd(lambda_i,d)} is the...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   a886_fix, a886_gcd, a886_mu, a886_mul, a886_powmul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a886_fix.h"
#include "../utils/a886_gcd.h"
#include "../utils/a886_mu.h"
#include "../utils/a886_mul.h"
#include "../utils/a886_powmul.h"

typedef struct {
    int N;
    uint64_t D;
    uint64_t fact;
    seq_t Q;
    unsigned __int128 R;
} a886_ctx;

static void a886_leaf(const int *parts, int k, a886_ctx *cx) {
    static const uint64_t cnt[5] = {1, 6, 3, 8, 6};
    uint64_t lcm = 1, z = 1;
    int i = 0;
    while (i < k) {
        int j = i;
        uint64_t mf = 1;
        while (j < k && parts[j] == parts[i]) {
            z *= (uint64_t)parts[j];
            mf *= (uint64_t)(j - i + 1);
            j++;
        }
        z *= mf;
        lcm = lcm / a886_gcd(lcm, (uint64_t)parts[i]) * (uint64_t)parts[i];
        i = j;
    }
    uint64_t w = cx->fact / z;

    uint64_t *Ls = (uint64_t *)malloc((size_t)(lcm + 1) * sizeof(uint64_t));
    uint64_t *Ns = (uint64_t *)malloc((size_t)(lcm + 1) * sizeof(uint64_t));
    int nl = 0;
    for (uint64_t L = 1; L <= lcm; L++) {
        if (lcm % L) continue;
        long long s = 0;
        for (uint64_t d = 1; d <= L; d++) {
            if (L % d) continue;
            int mu = a886_mu((long long)(L / d));
            if (!mu) continue;
            int c = 0;
            for (int t = 0; t < k; t++) c += (int)a886_gcd((uint64_t)parts[t], d);
            s += (long long)mu * (long long)((uint64_t)1 << c);
        }
        s /= (long long)L;
        if (s > 0) { Ls[nl] = L; Ns[nl] = (uint64_t)s; nl++; }
    }

    for (int t = 0; t < 5; t++) {
        int zero = 0;
        for (int l = 0; l < nl; l++)
            if (a886_fix(t, Ls[l]) == 0) { zero = 1; break; }
        if (zero) continue;
        uint64_t c0 = w * cnt[t];
        seq_t q = (seq_t)(c0 / cx->D);
        unsigned __int128 r = c0 % cx->D;
        for (int l = 0; l < nl; l++)
            a886_powmul(&q, &r, a886_fix(t, Ls[l]), Ns[l], cx->D);
        cx->R += r;
        cx->Q += q + (seq_t)(uint64_t)(cx->R / cx->D);
        cx->R %= cx->D;
    }
    free(Ls);
    free(Ns);
}

static void a886_rec(int rem, int maxp, int *parts, int k, a886_ctx *cx) {
    if (rem == 0) { a886_leaf(parts, k, cx); return; }
    for (int p = (rem < maxp ? rem : maxp); p >= 1; p--) {
        parts[k] = p;
        a886_rec(rem - p, p, parts, k + 1, cx);
    }
}

seq_t a(idx_t n) {
    if (n < 1 || n > 18) return 0;
    a886_ctx cx;
    cx.N = (int)n;
    cx.fact = 1;
    for (int i = 2; i <= cx.N; i++) cx.fact *= (uint64_t)i;
    cx.D = cx.fact * 24;
    cx.Q = 0;
    cx.R = 0;
    int parts[20];
    a886_rec(cx.N, cx.N, parts, 0, &cx);
    return cx.Q;
}
