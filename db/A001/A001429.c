/*
 * A001429  Number of n-node connected unicyclic graphs.
 * https://oeis.org/A001429
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = A068051(n) - A027852(n) - A000081(n), where with R(x) =
 *            Sum A000081(k) x^k, Q(x) = R(x^2), G = 1/(1-R), b = x R'(x)
 *            G(x): A068051(n) = (2*N(n) + [x^n] (2R + Q + R^2)/(1-Q)) / 4,
 *            N(n) = (1/n) Sum_{d|n} phi(d) b(n/d), A027852(n) = ([x^n] R^2 +
 *            [x^n] Q) / 2, A000081(n+1) = (1/n) Sum_{k=1..n}...
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 25, u64 n <= 46, u128 n <= 87,
 *            big any n
 * Helpers:   a1429_regrow, a1429_tot
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/a1429_regrow.h"
#include "../utils/a1429_tot.h"

static int a1429_mode = -1;

static int a1429_isbig(void)
{
    if (a1429_mode < 0) {
        seq_t x = 1;
        int i;
        for (i = 0; i < 200; i++) x += x;
        a1429_mode = (x != 0) ? 1 : 0;
    }
    return a1429_mode;
}

/* ---------- arbitrary-precision path ---------- */

static seq_t *BR = NULL, *BS = NULL, *BG = NULL, *BR2 = NULL;
static idx_t Bn = 0;

static void a1429_big_ensure(idx_t N)
{
    idx_t cap, m, k;
    if (N < Bn) return;
    cap = Bn + Bn / 2;
    if (cap < N + 1) cap = N + 1;
    BR = a1429_regrow(BR, Bn, cap);
    BS = a1429_regrow(BS, Bn, cap);
    BG = a1429_regrow(BG, Bn, cap);
    BR2 = a1429_regrow(BR2, Bn, cap);
    for (m = Bn; m < cap; m++) {
        if (m == 0) {
            BR[0] = 0; BS[0] = 0; BG[0] = 1; BR2[0] = 0;
            continue;
        }
        if (m == 1) {
            BR[1] = 1;
        } else {
            seq_t t = 0;
            for (k = 1; k < m; k++) t += BS[k] * BR[m - k];
            BR[m] = t / (seq_t)(m - 1);
        }
        {
            seq_t s = 0;
            for (k = 1; k <= m; k++)
                if (m % k == 0) s += (seq_t)k * BR[k];
            BS[m] = s;
        }
        {
            seq_t g = 0;
            for (k = 1; k <= m; k++) g += BR[k] * BG[m - k];
            BG[m] = g;
        }
        {
            seq_t q = 0;
            for (k = 1; k < m; k++) q += BR[k] * BR[m - k];
            BR2[m] = q;
        }
    }
    Bn = cap;
}

static seq_t a1429_big_calc(idx_t n)
{
    seq_t nsum = 0, Nn, T = 0, pos, neg, qn;
    idx_t d, k, i;
    a1429_big_ensure(n);
    for (d = 1; d <= n; d++) {
        if (n % d == 0) {
            idx_t m = n / d;
            seq_t b = 0;
            for (k = 1; k <= m; k++) b += (seq_t)k * BR[k] * BG[m - k];
            nsum += (seq_t)a1429_tot(d) * b;
        }
    }
    Nn = nsum / (seq_t)n;
    for (i = n % 2; i <= n; i += 2) {
        seq_t P = BR[i] + BR[i] + BR2[i];
        if (i % 2 == 0) P += BR[i / 2];
        T += P * BG[(n - i) / 2];
    }
    qn = (n % 2 == 0) ? BR[n / 2] : (seq_t)0;
    pos = Nn + Nn + T;
    neg = (seq_t)4 * BR[n] + (seq_t)2 * BR2[n] + (seq_t)2 * qn;
    return (pos - neg) / 4;
}

/* ---------- fixed-width path (256-bit intermediates) ---------- */

#define A1429_FMAX 200

typedef struct { uint32_t w[8]; } a1429_u;

static a1429_u a1429_U(uint64_t v)
{
    a1429_u r;
    memset(&r, 0, sizeof r);
    r.w[0] = (uint32_t)v;
    r.w[1] = (uint32_t)(v >> 32);
    return r;
}

static a1429_u a1429_add(a1429_u x, a1429_u y)
{
    a1429_u r;
    uint64_t c = 0;
    int i;
    for (i = 0; i < 8; i++) {
        c += (uint64_t)x.w[i] + y.w[i];
        r.w[i] = (uint32_t)c;
        c >>= 32;
    }
    return r;
}

static a1429_u a1429_sub(a1429_u x, a1429_u y)
{
    a1429_u r;
    uint64_t br = 0;
    int i;
    for (i = 0; i < 8; i++) {
        uint64_t t = (uint64_t)x.w[i] - (uint64_t)y.w[i] - br;
        r.w[i] = (uint32_t)t;
        br = (t >> 32) ? 1 : 0;
    }
    return r;
}

static a1429_u a1429_mul(a1429_u x, a1429_u y)
{
    a1429_u r;
    int i, j;
    memset(&r, 0, sizeof r);
    for (i = 0; i < 8; i++) {
        uint64_t c = 0;
        if (x.w[i] == 0) continue;
        for (j = 0; i + j < 8; j++) {
            c += (uint64_t)x.w[i] * y.w[j] + r.w[i + j];
            r.w[i + j] = (uint32_t)c;
            c >>= 32;
        }
    }
    return r;
}

static a1429_u a1429_div(a1429_u x, uint32_t dv)
{
    a1429_u r;
    uint64_t rem = 0;
    int i;
    for (i = 7; i >= 0; i--) {
        uint64_t cur = (rem << 32) | x.w[i];
        r.w[i] = (uint32_t)(cur / dv);
        rem = cur % dv;
    }
    return r;
}

static seq_t a1429_to_seq(a1429_u x)
{
    seq_t v = 0;
    int i;
    for (i = 7; i >= 0; i--) {
        v <<= 8; v <<= 8; v <<= 8; v <<= 8;
        v += (seq_t)x.w[i];
    }
    return v;
}

static a1429_u FR[A1429_FMAX + 1], FS[A1429_FMAX + 1], FG[A1429_FMAX + 1], FR2[A1429_FMAX + 1];
static int Fdone = 0;

static void a1429_fix_init(void)
{
    idx_t m, k;
    if (Fdone) return;
    FR[0] = a1429_U(0); FS[0] = a1429_U(0); FG[0] = a1429_U(1); FR2[0] = a1429_U(0);
    for (m = 1; m <= A1429_FMAX; m++) {
        a1429_u t;
        if (m == 1) {
            FR[1] = a1429_U(1);
        } else {
            t = a1429_U(0);
            for (k = 1; k < m; k++) t = a1429_add(t, a1429_mul(FS[k], FR[m - k]));
            FR[m] = a1429_div(t, (uint32_t)(m - 1));
        }
        t = a1429_U(0);
        for (k = 1; k <= m; k++)
            if (m % k == 0) t = a1429_add(t, a1429_mul(a1429_U(k), FR[k]));
        FS[m] = t;
        t = a1429_U(0);
        for (k = 1; k <= m; k++) t = a1429_add(t, a1429_mul(FR[k], FG[m - k]));
        FG[m] = t;
        t = a1429_U(0);
        for (k = 1; k < m; k++) t = a1429_add(t, a1429_mul(FR[k], FR[m - k]));
        FR2[m] = t;
    }
    Fdone = 1;
}

static seq_t a1429_fix_calc(idx_t n)
{
    a1429_u nsum = a1429_U(0), Nn, T = a1429_U(0), pos, neg, qn;
    idx_t d, k, i;
    a1429_fix_init();
    for (d = 1; d <= n; d++) {
        if (n % d == 0) {
            idx_t m = n / d;
            a1429_u b = a1429_U(0);
            for (k = 1; k <= m; k++)
                b = a1429_add(b, a1429_mul(a1429_mul(a1429_U(k), FR[k]), FG[m - k]));
            nsum = a1429_add(nsum, a1429_mul(a1429_U(a1429_tot(d)), b));
        }
    }
    Nn = a1429_div(nsum, (uint32_t)n);
    for (i = n % 2; i <= n; i += 2) {
        a1429_u P = a1429_add(a1429_add(FR[i], FR[i]), FR2[i]);
        if (i % 2 == 0) P = a1429_add(P, FR[i / 2]);
        T = a1429_add(T, a1429_mul(P, FG[(n - i) / 2]));
    }
    qn = (n % 2 == 0) ? FR[n / 2] : a1429_U(0);
    pos = a1429_add(a1429_add(Nn, Nn), T);
    neg = a1429_add(a1429_add(a1429_mul(a1429_U(4), FR[n]), a1429_mul(a1429_U(2), FR2[n])),
                    a1429_mul(a1429_U(2), qn));
    return a1429_to_seq(a1429_div(a1429_sub(pos, neg), 4));
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;
    if (a1429_isbig()) return a1429_big_calc(n);
    /* beyond this a(n) exceeds 2^256, so it cannot fit any fixed-width seq_t */
    if (n > A1429_FMAX) return 0;
    return a1429_fix_calc(n);
}
