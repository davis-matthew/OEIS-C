/*
 * A203816  Decimal expansion of e*gamma*Pi, where gamma is Euler's constant.
 * https://oeis.org/A203816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n-1) * e*gamma*Pi) mod 10, with a(1) the digit
 *            4 before the decimal point. Computed with base-1e9 fixed-point
 *            big arithmetic: Brent-McMillan for gamma, Machin for Pi, series
 *            for e
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 114 terms through n = 8183 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BS ((uint64_t)1000000000)

static int g_L, g_F, g_I;
static uint32_t *g_res = 0;
static idx_t g_digits = 0;

static uint32_t *newarr(int n) { return (uint32_t *)calloc((size_t)n, sizeof(uint32_t)); }

static void mul_small(uint32_t *a, int lo, uint64_t m) {
    uint64_t carry = 0;
    for (int i = g_L - 1; i >= lo; i--) {
        uint64_t cur = (uint64_t)a[i] * m + carry;
        a[i] = (uint32_t)(cur % BS);
        carry = cur / BS;
    }
}
static void div_small(uint32_t *a, int lo, uint64_t d) {
    uint64_t rem = 0;
    for (int i = lo; i < g_L; i++) {
        uint64_t cur = rem * BS + a[i];
        a[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
}
static void add_arr(uint32_t *dst, const uint32_t *src, int lo) {
    uint64_t carry = 0;
    for (int i = g_L - 1; i >= lo; i--) {
        uint64_t c = (uint64_t)dst[i] + src[i] + carry;
        if (c >= BS) { c -= BS; carry = 1; } else carry = 0;
        dst[i] = (uint32_t)c;
    }
    for (int i = lo - 1; carry && i >= 0; i--) {
        if (dst[i] + 1 == BS) dst[i] = 0; else { dst[i]++; carry = 0; }
    }
}
static void sub_arr(uint32_t *dst, const uint32_t *src, int lo) {
    int64_t borrow = 0;
    for (int i = g_L - 1; i >= lo; i--) {
        int64_t d = (int64_t)dst[i] - (int64_t)src[i] - borrow;
        if (d < 0) { d += (int64_t)BS; borrow = 1; } else borrow = 0;
        dst[i] = (uint32_t)d;
    }
    for (int i = lo - 1; borrow && i >= 0; i--) {
        if (dst[i] == 0) dst[i] = (uint32_t)(BS - 1); else { dst[i]--; borrow = 0; }
    }
}

/* R = sum_j (alt?(-1)^j:1) (p/q)^(2j+1)/(2j+1) */
static void series(uint32_t *R, uint64_t p, uint64_t q, int alt) {
    int L = g_L;
    memset(R, 0, sizeof(uint32_t) * (size_t)L);
    if (p == 0) return;
    uint32_t *t = newarr(L), *u = newarr(L), *Rn = newarr(L);
    t[g_I - 1] = (uint32_t)p;
    div_small(t, g_I - 1, q);
    int s = g_I - 1;
    while (s < L && t[s] == 0) s++;
    uint64_t p2 = p * p, q2 = q * q;
    for (uint64_t j = 0; s < L; j++) {
        memcpy(u + s, t + s, sizeof(uint32_t) * (size_t)(L - s));
        div_small(u, s, 2 * j + 1);
        if (alt && (j & 1)) add_arr(Rn, u, s); else add_arr(R, u, s);
        int lo = s > 0 ? s - 1 : 0;
        mul_small(t, lo, p2);
        div_small(t, lo, q2);
        s = lo;
        while (s < L && t[s] == 0) s++;
    }
    if (alt) sub_arr(R, Rn, 0);
    free(t); free(u); free(Rn);
}

static void fmul(const uint32_t *a, const uint32_t *b, uint32_t *out) {
    int L = g_L;
    uint32_t *res = newarr(2 * L);
    for (int i = L - 1; i >= 0; i--) {
        uint64_t carry = 0, ai = a[i];
        if (ai) {
            for (int j = L - 1; j >= 0; j--) {
                uint64_t cur = (uint64_t)res[i + j + 1] + ai * b[j] + carry;
                res[i + j + 1] = (uint32_t)(cur % BS);
                carry = cur / BS;
            }
        }
        res[i] = (uint32_t)carry;
    }
    for (int i = 0; i < L; i++) out[i] = res[g_I + i];
    free(res);
}

/* G = SP/V (fixed point) */
static void long_div(const uint32_t *X, const uint32_t *V, uint32_t *G) {
    int L = g_L, F = g_F;
    int vs = 0;
    while (vs < L && V[vs] == 0) vs++;
    int m = L - vs;
    const uint32_t *v = V + vs;
    int total = L + F;
    uint32_t *Q = newarr(total);
    int64_t *rem = (int64_t *)calloc((size_t)m + 1, sizeof(int64_t));
    uint64_t vt = (uint64_t)v[0] * BS + v[1];
    for (int t = 0; t < total; t++) {
        uint32_t xl = t < L ? X[t] : 0;
        for (int j = 0; j < m; j++) rem[j] = rem[j + 1];
        rem[m] = xl;
        unsigned __int128 rt = (unsigned __int128)rem[0] * BS * BS + (unsigned __int128)rem[1] * BS + (unsigned __int128)rem[2];
        uint64_t qh = (uint64_t)(rt / vt);
        if (qh >= BS) qh = BS - 1;
        if (qh) {
            uint64_t carry = 0; int64_t borrow = 0;
            for (int j = m - 1; j >= 0; j--) {
                uint64_t pr = qh * v[j] + carry;
                carry = pr / BS;
                int64_t d = rem[j + 1] - (int64_t)(pr % BS) - borrow;
                if (d < 0) { d += (int64_t)BS; borrow = 1; } else borrow = 0;
                rem[j + 1] = d;
            }
            rem[0] = rem[0] - (int64_t)carry - borrow;
        }
        while (rem[0] < 0) {
            int64_t c = 0;
            for (int j = m - 1; j >= 0; j--) {
                int64_t sm = rem[j + 1] + v[j] + c;
                if (sm >= (int64_t)BS) { sm -= (int64_t)BS; c = 1; } else c = 0;
                rem[j + 1] = sm;
            }
            rem[0] += c;
            qh--;
        }
        for (;;) {
            bool ge;
            if (rem[0] > 0) ge = true;
            else {
                ge = true;
                for (int j = 0; j < m; j++) {
                    if (rem[j + 1] != (int64_t)v[j]) { ge = rem[j + 1] > (int64_t)v[j]; break; }
                }
            }
            if (!ge) break;
            int64_t borrow = 0;
            for (int j = m - 1; j >= 0; j--) {
                int64_t d = rem[j + 1] - (int64_t)v[j] - borrow;
                if (d < 0) { d += (int64_t)BS; borrow = 1; } else borrow = 0;
                rem[j + 1] = d;
            }
            rem[0] -= borrow;
            qh++;
        }
        Q[t] = (uint32_t)qh;
    }
    for (int i = 0; i < L; i++) G[i] = Q[F + i];
    free(Q); free(rem);
}

static void norm64(const uint64_t *a, uint32_t *out) {
    uint64_t carry = 0;
    for (int i = g_L - 1; i >= 0; i--) {
        uint64_t c = a[i] + carry;
        out[i] = (uint32_t)(c % BS);
        carry = c / BS;
    }
}

static void compute(idx_t digits) {
    int Dg = (int)digits + 20;
    int F = Dg / 9 + 3;
    int nn = (int)(((int64_t)Dg * 5758) / 10000) + 5;
    int idig = nn * 87 / 100 + 40;
    int I = idig / 9 + 3;
    int L = I + F;
    g_L = L; g_F = F; g_I = I;

    /* Brent-McMillan */
    uint32_t *B = newarr(L), *P = newarr(L);
    uint64_t *SPa = (uint64_t *)calloc((size_t)L, sizeof(uint64_t));
    uint64_t *Va = (uint64_t *)calloc((size_t)L, sizeof(uint64_t));
    B[I - 1] = 1; Va[I - 1] = 1;
    int s = I - 1;
    uint64_t n2 = (uint64_t)nn * (uint64_t)nn;
    for (uint64_t k = 1;; k++) {
        int lo = s > 0 ? s - 1 : 0;
        uint64_t k2 = k * k;
        mul_small(B, lo, n2);
        div_small(B, lo, k2);
        uint64_t carry = 0;
        for (int i = L - 1; i >= lo; i--) {
            uint64_t cur = (uint64_t)P[i] * n2 + (uint64_t)B[i] * k + carry;
            P[i] = (uint32_t)(cur % BS);
            carry = cur / BS;
        }
        div_small(P, lo, k2);
        for (int i = lo; i < L; i++) { Va[i] += B[i]; SPa[i] += P[i]; }
        s = lo;
        while (s < L && B[s] == 0 && P[s] == 0) s++;
        if (s >= L) break;
    }
    uint32_t *SP = newarr(L), *V = newarr(L);
    norm64(SPa, SP); norm64(Va, V);
    free(SPa); free(Va); free(B); free(P);
    uint32_t *G = newarr(L);
    long_div(SP, V, G);
    free(SP); free(V);

    /* ln(nn) */
    uint32_t *ln2 = newarr(L), *T = newarr(L), *LN = newarr(L);
    series(ln2, 1, 3, 0);
    mul_small(ln2, 0, 2);
    int kk = 0;
    while (((uint64_t)1 << (kk + 1)) <= (uint64_t)nn) kk++;
    uint64_t pw = (uint64_t)1 << kk;
    series(T, (uint64_t)nn - pw, (uint64_t)nn + pw, 0);
    mul_small(T, 0, 2);
    memcpy(LN, ln2, sizeof(uint32_t) * (size_t)L);
    mul_small(LN, 0, (uint64_t)kk);
    add_arr(LN, T, 0);
    sub_arr(G, LN, 0);
    free(ln2); free(T); free(LN);

    /* Pi */
    uint32_t *a5 = newarr(L), *a239 = newarr(L);
    series(a5, 1, 5, 1);
    series(a239, 1, 239, 1);
    mul_small(a5, 0, 16);
    mul_small(a239, 0, 4);
    sub_arr(a5, a239, 0);
    free(a239);

    /* e */
    uint32_t *E = newarr(L), *t = newarr(L);
    E[I - 1] = 1; t[I - 1] = 1;
    s = I - 1;
    for (uint64_t k = 1; s < L; k++) {
        div_small(t, s, k);
        add_arr(E, t, s);
        while (s < L && t[s] == 0) s++;
    }
    free(t);

    uint32_t *X = newarr(L), *Y = newarr(L);
    fmul(E, G, X);
    fmul(X, a5, Y);
    free(X); free(E); free(G); free(a5);
    if (g_res) free(g_res);
    g_res = Y;
    g_digits = digits;
}

seq_t a(idx_t n) {
    if (n > g_digits || !g_res) {
        idx_t want = n + n / 8 + 32;
        compute(want);
    }
    int I = g_I;
    uint32_t dig;
    if (n == 1) {
        dig = g_res[I - 1] % 10;
    } else {
        uint64_t d = (uint64_t)(n - 1);
        uint64_t limb = (uint64_t)(I - 1) + (d + 8) / 9;
        int pos = (int)((d - 1) % 9);
        uint32_t div = 1;
        for (int i = 0; i < 8 - pos; i++) div *= 10;
        dig = (g_res[limb] / div) % 10;
    }
    return (seq_t)dig;
}
