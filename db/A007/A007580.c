/*
 * A007580  Number of Young tableaux of height <= 8.
 * https://oeis.org/A007580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Gessel's determinant: sum_{n>=0} a(n) x^n/n! = det[ I_{i-j}(2x)
 *            + I_{i+j-1}(2x) ]_{i,j=1..4}, where I_k(2x) = sum_n
 *            binomial(n,(n-k)/2) x^n/n!. Products of two such series have the
 *            closed form I_a(2x)*I_b(2x) = sum_n
 *            binomial(n,(n-a-b)/2)*binomial(n,(n-a+b)/2) x^n/n!, so a Laplace
 *            expansion along...
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 32, u128 n <= 56,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

typedef struct {
    seq_t c[15];
    seq_t pr[15][15];
    bool have[15][15];
} Ctx;

static Ctx CA, CB;

/* c[s] = binomial(m, (m-s)/2) when m-s is even and s <= m, else 0 */
static void ctx_set(Ctx *x, idx_t m, seq_t cm) {
    int s, t;
    long long k = (long long)(m / 2);
    seq_t v = cm;
    for (s = 0; s < 15; s++) {
        x->c[s] = 0;
        for (t = 0; t < 15; t++) x->have[s][t] = false;
    }
    for (s = (int)(m % 2); s < 15; s += 2) {
        x->c[s] = v;
        if (k <= 0) v = 0;
        else v = v * (seq_t)(idx_t)k / (seq_t)(m - (idx_t)k + 1);
        k--;
    }
}

static seq_t P(Ctx *x, int a, int b) {
    int s = abs(a + b), d = abs(a - b);
    if (s > d) { int t = s; s = d; d = t; }
    if (!x->have[s][d]) {
        if (x->c[s] == 0 || x->c[d] == 0) x->pr[s][d] = 0;
        else x->pr[s][d] = x->c[s] * x->c[d];
        x->have[s][d] = true;
    }
    return x->pr[s][d];
}

/* coefficient of E(r1,p)*E(r2,q), E(i,j) = I_{i-j} + I_{i+j-1} */
static seq_t EE(Ctx *x, int r1, int p, int r2, int q) {
    int xs[2], ys[2], u, w;
    seq_t r = 0;
    xs[0] = r1 - p; xs[1] = r1 + p - 1;
    ys[0] = r2 - q; ys[1] = r2 + q - 1;
    for (u = 0; u < 2; u++)
        for (w = 0; w < 2; w++)
            r += P(x, xs[u], ys[w]);
    return r;
}

seq_t a(idx_t n) {
    static const int PQ[6][5] = {
        {1, 2, 3, 4, 1}, {1, 3, 2, 4, -1}, {1, 4, 2, 3, 1},
        {2, 3, 1, 4, 1}, {2, 4, 1, 3, -1}, {3, 4, 1, 2, 1}
    };
    seq_t *cen = alloc_zero(n + 1);
    seq_t pos = 0, neg = 0, bc = 1;
    idx_t m, i;
    int k;

    cen[0] = 1;
    for (m = 0; m < n; m++) {
        if (m % 2 == 0) cen[m + 1] = cen[m] * (seq_t)(m + 1) / (seq_t)(m / 2 + 1);
        else cen[m + 1] = cen[m] * 2;
    }

    for (i = 0; i <= n; i++) {
        seq_t pi = 0, ni = 0;
        ctx_set(&CA, i, cen[i]);
        ctx_set(&CB, n - i, cen[n - i]);
        for (k = 0; k < 6; k++) {
            int p = PQ[k][0], q = PQ[k][1], r = PQ[k][2], s = PQ[k][3];
            bool negative = PQ[k][4] < 0;
            seq_t X1 = EE(&CA, 1, p, 2, q), Y1 = EE(&CA, 1, q, 2, p);
            seq_t D1, D2, X2, Y2;
            if (X1 == Y1) continue;
            if (X1 >= Y1) D1 = X1 - Y1;
            else { D1 = Y1 - X1; negative = !negative; }
            X2 = EE(&CB, 3, r, 4, s);
            Y2 = EE(&CB, 3, s, 4, r);
            if (X2 == Y2) continue;
            if (X2 >= Y2) D2 = X2 - Y2;
            else { D2 = Y2 - X2; negative = !negative; }
            if (negative) ni += D1 * D2;
            else pi += D1 * D2;
        }
        if (pi != 0) pos += bc * pi;
        if (ni != 0) neg += bc * ni;
        if (i < n) bc = bc * (seq_t)(n - i) / (seq_t)(i + 1);
    }

    release(cen, n + 1);
    return pos - neg;
}
