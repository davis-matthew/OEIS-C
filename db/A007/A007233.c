/*
 * A007233  Number of free subsets of multiplicative group of GF(7^n).
 * https://oeis.org/A007233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max size of S in GF(7^n)* containing no pair x, x^2.
 *            Write 7^n-1 = 2^e*o, o odd. Then a(n) = o*Sum_{j even, 0<=j<e}
 *            2^(e-1-j), plus (o - C)/2 when e is even. Here C = number of
 *            odd-length cycles of x -> 2x on Z_o = Sum_{d|h} phi(d)/ord_d(2),
 *            where h is the largest divisor of o with ord_h(2...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   addf, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/seq_is_big.h"
#include "../utils/addf.h"

static seq_t sg(seq_t x, seq_t y) {
    while (y != 0) { seq_t t = x % y; x = y; y = t; }
    return x;
}

static seq_t mulm(seq_t x, seq_t y, seq_t m) {
    if (seq_is_big()) return (x * y) % m;
    seq_t r = 0;
    x %= m;
    while (y > 0) {
        if ((y & 1) != 0) { r += x; if (r >= m) r -= m; }
        x += x; if (x >= m) x -= m;
        y >>= 1;
    }
    return r;
}

static seq_t powm(seq_t b, seq_t e, seq_t m) {
    seq_t r = 1 % m;
    b %= m;
    while (e > 0) {
        if ((e & 1) != 0) r = mulm(r, b, m);
        b = mulm(b, b, m);
        e >>= 1;
    }
    return r;
}

static bool isprp(seq_t n) {
    static const unsigned bases[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    int i, s = 0, j;
    seq_t d;
    if (n < 2) return false;
    for (i = 0; i < 12; i++) {
        if (n == (seq_t)bases[i]) return true;
        if (n % bases[i] == 0) return false;
    }
    d = n - 1;
    while (d % 2 == 0) { d /= 2; s++; }
    for (i = 0; i < 12; i++) {
        seq_t x = powm((seq_t)bases[i], d, n);
        bool ok;
        if (x == 1 || x == n - 1) continue;
        ok = false;
        for (j = 1; j < s; j++) {
            x = mulm(x, x, n);
            if (x == n - 1) { ok = true; break; }
        }
        if (!ok) return false;
    }
    return true;
}

static seq_t fstep(seq_t y, seq_t c, seq_t n) { return (mulm(y, y, n) + c) % n; }

static seq_t rho(seq_t n) {
    seq_t c = 1;
    for (;;) {
        seq_t y = 2, x = 2, q = 1, g = 1, ys = 2;
        idx_t r = 1, k, i, lim, m = 64;
        do {
            x = y;
            for (i = 0; i < r; i++) y = fstep(y, c, n);
            k = 0;
            do {
                ys = y;
                lim = (m < r - k) ? m : r - k;
                for (i = 0; i < lim; i++) {
                    y = fstep(y, c, n);
                    q = mulm(q, x > y ? x - y : y - x, n);
                }
                g = sg(q, n);
                k += m;
            } while (k < r && g == 1);
            r *= 2;
        } while (g == 1);
        if (g == n) {
            do {
                ys = fstep(ys, c, n);
                g = sg(x > ys ? x - ys : ys - x, n);
            } while (g == 1);
        }
        if (g != n) return g;
        c += 1;
    }
}

static void frec(seq_t n, seq_t *lst, int *cnt, int cap) {
    seq_t d;
    if (n == 1) return;
    if (isprp(n)) { addf(n, lst, cnt, cap); return; }
    d = rho(n);
    frec(d, lst, cnt, cap);
    frec(n / d, lst, cnt, cap);
}

static void factor_into(seq_t n, seq_t *lst, int *cnt, int cap) {
    uint64_t p;
    for (p = 2; p < 1000; p++) {
        seq_t sp = (seq_t)p;
        if (sp > n / sp) break;
        while (n % sp == 0) { addf(sp, lst, cnt, cap); n /= sp; }
    }
    if (n > 1) frec(n, lst, cnt, cap);
}

#define FCAP 1024
#define TCAP 512
#define HCAP 64
#define ECAP 48

static seq_t g_fac[FCAP];
static seq_t g_tmp[TCAP];
static seq_t hph[HCAP][ECAP], hod[HCAP][ECAP];
static int hexp[HCAP];
static int nh;
static seq_t gC;

static void dsum(int i, seq_t ph, seq_t od) {
    int b;
    if (i == nh) { gC += ph / od; return; }
    dsum(i + 1, ph, od);
    for (b = 1; b <= hexp[i]; b++) {
        seq_t o2 = hod[i][b];
        seq_t l = od / sg(od, o2) * o2;
        dsum(i + 1, ph * hph[i][b], l);
    }
}

seq_t a(idx_t n) {
    idx_t i, d;
    int e = 0, j, nf = 0, x, y;
    seq_t m = 1, o, S = 0, res, rem, pw;

    if (n % 2 == 1) {
        seq_t s = 0;
        for (i = 0; i < n; i++) s = s * 7 + 1;
        return s * 3;
    }
    for (i = 0; i < n; i++) {
        seq_t nx = m * 7;
        if (nx / 7 != m) return 0; /* fixed-width overflow: term cannot fit */
        m = nx;
    }
    m -= 1;
    o = m;
    while (o % 2 == 0) { o /= 2; e++; }
    for (j = 0; j < e; j += 2) S += (seq_t)1 << (e - 1 - j);
    res = o * S;
    if (e % 2 == 1) return res;

    /* factor o using pieces gcd(rem, 7^d - 1) for d | n */
    rem = o;
    pw = 1;
    for (d = 1; d <= n; d++) {
        pw *= 7;
        if (n % d == 0 && rem > 1) {
            seq_t g = sg(rem, pw - 1);
            if (g > 1) { factor_into(g, g_fac, &nf, FCAP); rem /= g; }
        }
    }
    if (rem > 1) factor_into(rem, g_fac, &nf, FCAP);
    for (x = 1; x < nf; x++) {
        seq_t v = g_fac[x];
        y = x - 1;
        while (y >= 0 && g_fac[y] > v) { g_fac[y + 1] = g_fac[y]; y--; }
        g_fac[y + 1] = v;
    }

    nh = 0;
    x = 0;
    while (x < nf) {
        seq_t p = g_fac[x], r, t, pk;
        int cnt = 0, nt = 0, b;
        while (x < nf && g_fac[x] == p) { cnt++; x++; }
        r = p - 1;
        while (r % 2 == 0) r /= 2;
        if (powm((seq_t)2, r, p) != 1) continue; /* ord_p(2) even */
        factor_into(r, g_tmp, &nt, TCAP);
        t = r;
        for (y = 0; y < nt; y++)
            if (powm((seq_t)2, t / g_tmp[y], p) == 1) t /= g_tmp[y];
        if (nh >= HCAP) continue;
        if (cnt >= ECAP) cnt = ECAP - 1;
        hexp[nh] = cnt;
        hph[nh][1] = p - 1;
        hod[nh][1] = t;
        pk = p;
        for (b = 2; b <= cnt; b++) {
            pk *= p;
            if (powm((seq_t)2, t, pk) != 1) t *= p;
            hod[nh][b] = t;
            hph[nh][b] = hph[nh][b - 1] * p;
        }
        nh++;
    }
    gC = 0;
    dsum(0, (seq_t)1, (seq_t)1);
    res += (o - gC) / 2;
    return res;
}
