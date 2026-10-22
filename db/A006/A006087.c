/*
 * A006087  Unitary harmonic means H(n) of the unitary harmonic numbers
 *          (A006086).
 * https://oeis.org/A006087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = A103339(A006086(n)) = m*ud(m)/usigma(m) with m =
 *            A006086(n); the unitary harmonic numbers m are found by a pruned
 *            depth-first search over their prime-power factorizations (primes
 *            added in increasing order, every odd prime of usigma(m) must
 *            divide m), for all m <= X with X increased as needed
 * Range:     u8 n <= 248, u16 n <= 290, u32 n <= 290, u64 n <= 290,
 *            u128 n <= 290, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned __int128 u128_t;

static uint64_t X = 0;
static uint32_t *pr = NULL;
static size_t npr = 0;
static uint64_t *rn = NULL, *rh = NULL;
static size_t rc = 0, rcap = 0;
static uint64_t opr[64];
static int nop = 0;

static uint64_t isq(uint64_t x) {
    uint64_t r = 0, b = (uint64_t)1 << 31;
    while (b) {
        uint64_t t = r + b;
        if (t * t <= x) r = t;
        b >>= 1;
    }
    return r;
}

static uint64_t g_gcd(uint64_t x, uint64_t y) {
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static uint64_t mulm(uint64_t x, uint64_t y, uint64_t m) {
    return (uint64_t)((u128_t)x * y % m);
}

static uint64_t powm(uint64_t b, uint64_t e, uint64_t m) {
    uint64_t r = 1;
    b %= m;
    while (e) {
        if (e & 1) r = mulm(r, b, m);
        b = mulm(b, b, m);
        e >>= 1;
    }
    return r;
}

static int mr(uint64_t n) {
    static const uint64_t bs[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    uint64_t d;
    int s = 0, i, j;
    if (n < 2) return 0;
    for (i = 0; i < 12; i++) {
        if (n == bs[i]) return 1;
        if (n % bs[i] == 0) return 0;
    }
    d = n - 1;
    while (!(d & 1)) { d >>= 1; s++; }
    for (i = 0; i < 12; i++) {
        uint64_t x = powm(bs[i], d, n);
        if (x == 1 || x == n - 1) continue;
        for (j = 1; j < s; j++) {
            x = mulm(x, x, n);
            if (x == n - 1) break;
        }
        if (j == s) return 0;
    }
    return 1;
}

static void sieve(uint64_t lim) {
    char *c = (char *)calloc((size_t)lim + 1, 1);
    uint64_t i, j;
    size_t cnt = 0;
    free(pr);
    pr = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)lim / 2 + 2));
    for (i = 2; i <= lim; i++) {
        if (!c[i]) {
            pr[cnt++] = (uint32_t)i;
            for (j = i * i; j <= lim; j += i) c[j] = 1;
        }
    }
    free(c);
    npr = cnt;
}

static void record(uint64_t n, uint64_t h) {
    if (rc == rcap) {
        rcap = rcap ? rcap * 2 : 256;
        rn = (uint64_t *)realloc(rn, sizeof(uint64_t) * rcap);
        rh = (uint64_t *)realloc(rh, sizeof(uint64_t) * rcap);
    }
    rn[rc] = n;
    rh[rc] = h;
    rc++;
}

static void reduce(uint64_t *x, uint64_t *s) {
    uint64_t g = g_gcd(*x, *s);
    while (g > 1) {
        *x /= g;
        *s /= g;
        g = g_gcd(*x, *s);
    }
}

static void dfs(uint64_t n, uint64_t nodd, uint64_t S, uint64_t O, uint64_t maxp,
                int w, int t, int e2, u128_t sig);

static void try_add(uint64_t n, uint64_t nodd, uint64_t S, uint64_t O, int w, int t,
                    int e2, u128_t sig, uint64_t p, uint64_t q) {
    uint64_t n2 = n * q, nodd2 = nodd * q, S2 = S * q, f = q + 1, O2 = O;
    int tt = 0;
    u128_t no;
    while (!(f & 1)) { f >>= 1; tt++; }
    reduce(&O2, &S2);
    reduce(&f, &S2);
    no = (u128_t)n2 * O2;
    if (no > X) return;
    if (no * f > X) return;
    O2 *= f;
    if (O2 > 1 && g_gcd(O2, nodd2) > 1) return;
    opr[nop++] = p;
    dfs(n2, nodd2, S2, O2, p, w + 1, t + tt, e2, sig * (u128_t)(q + 1));
    nop--;
}

static void try_powers(uint64_t n, uint64_t nodd, uint64_t S, uint64_t O, int w, int t,
                       int e2, u128_t sig, uint64_t p) {
    uint64_t q;
    if ((u128_t)p * p > X / n) return;
    q = p * p;
    while ((u128_t)n * q <= X) {
        try_add(n, nodd, S, O, w, t, e2, sig, p, q);
        if (q > X / p) break;
        q *= p;
    }
}

static void dfs(uint64_t n, uint64_t nodd, uint64_t S, uint64_t O, uint64_t maxp,
                int w, int t, int e2, u128_t sig) {
    uint64_t qs[64], cnt[64], c[64];
    uint64_t d, L, L1, lim, xn = X / n;
    int i, k;
    size_t lo, hi, j;

    if (O == 1 && t <= e2 + w)
        record(n, (uint64_t)((((u128_t)n) << w) / sig));

    /* new prime with exponent 1: p + 1 = 2^k * d, d | S */
    for (i = 0; i < nop; i++) {
        uint64_t s = S;
        qs[i] = opr[i];
        c[i] = 0;
        cnt[i] = 0;
        while (s % qs[i] == 0) { s /= qs[i]; c[i]++; }
    }
    d = 1;
    for (;;) {
        if (d <= xn + 1) {
            for (k = 1; k < 64; k++) {
                u128_t mm = ((u128_t)d << k) - 1;
                uint64_t m;
                if (mm > xn) break;
                m = (uint64_t)mm;
                if (m <= maxp) continue;
                if (O > 1 && m > O) break;
                if (mr(m)) try_add(n, nodd, S, O, w, t, e2, sig, m, m);
            }
        }
        i = 0;
        while (i < nop) {
            if (cnt[i] < c[i]) { cnt[i]++; d *= qs[i]; break; }
            while (cnt[i] > 0) { d /= qs[i]; cnt[i]--; }
            i++;
        }
        if (i == nop) break;
    }

    /* new prime with exponent >= 2 */
    L = isq(xn);
    L1 = L;
    if (O > 1) L1 = isq(xn / O);
    lim = L < L1 ? L : L1;
    lo = 0;
    hi = npr;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (pr[mid] <= maxp) lo = mid + 1; else hi = mid;
    }
    for (j = lo; j < npr && pr[j] <= lim; j++)
        try_powers(n, nodd, S, O, w, t, e2, sig, pr[j]);
    if (O > 1) {
        uint64_t b = L1 > maxp ? L1 : maxp;
        if (mr(O)) {
            if (O > b && O <= L) try_powers(n, nodd, S, O, w, t, e2, sig, O);
        } else {
            uint64_t h2 = isq(O);
            if (h2 > L) h2 = L;
            for (j = lo; j < npr && pr[j] <= h2; j++) {
                if (pr[j] <= b) continue;
                if (O % pr[j] == 0) try_powers(n, nodd, S, O, w, t, e2, sig, pr[j]);
            }
        }
    }
}

static int cmp_idx(const void *x, const void *y) {
    uint64_t u = rn[*(const size_t *)x], v = rn[*(const size_t *)y];
    return u < v ? -1 : (u > v ? 1 : 0);
}

static void search(void) {
    int e;
    size_t i, *ord;
    uint64_t *tn, *th;
    sieve(isq(X) + 2);
    rc = 0;
    nop = 0;
    dfs(1, 1, 1, 1, 2, 0, 0, 0, (u128_t)1);
    for (e = 1; e < 63; e++) {
        uint64_t q = (uint64_t)1 << e, O = q + 1;
        if ((u128_t)q * O > X) break;
        dfs(q, 1, 1, O, 2, 1, 0, e, (u128_t)(q + 1));
    }
    ord = (size_t *)malloc(sizeof(size_t) * (rc + 1));
    tn = (uint64_t *)malloc(sizeof(uint64_t) * (rc + 1));
    th = (uint64_t *)malloc(sizeof(uint64_t) * (rc + 1));
    for (i = 0; i < rc; i++) ord[i] = i;
    qsort(ord, rc, sizeof(size_t), cmp_idx);
    for (i = 0; i < rc; i++) { tn[i] = rn[ord[i]]; th[i] = rh[ord[i]]; }
    memcpy(rn, tn, sizeof(uint64_t) * rc);
    memcpy(rh, th, sizeof(uint64_t) * rc);
    free(ord);
    free(tn);
    free(th);
}

seq_t a(idx_t n) {
    if (X == 0) {
        X = (uint64_t)1 << 20;
        search();
    }
    while ((idx_t)rc < n) {
        X *= 8;
        search();
    }
    return (seq_t)rh[n - 1];
}
