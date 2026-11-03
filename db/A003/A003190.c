/*
 * A003190  Number of connected 2-plexes.
 * https://oeis.org/A003190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = inverse Euler transform of A000665, where A000665(n) =
 *            (1/n!) * Sum_{partitions p of n} (n!/z(p)) * 2^{c(p)}, z(p) =
 *            Prod k^{m_k} m_k!, c(p) = number of cycles induced by a
 *            permutation of cycle type p on the 3-subsets of an n-set.
 *            Inverse Euler transform: c(k) = k*b(k) - Sum_{i=1..k-1} c(i...
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd_u64.h"

/* three elements from one cycle of length a */
static uint64_t a3190_f3(uint64_t a)
{
    uint64_t t = a * (a - 1) * (a - 2) / 6;
    if (a % 3 == 0) t += 2 * (a / 3);
    return t / a;
}

static seq_t a3190_from128(unsigned __int128 v)
{
    seq_t t32 = (seq_t)65536 * (seq_t)65536;
    return (seq_t)(uint64_t)v + (seq_t)(uint64_t)(v >> 64) * (t32 * t32);
}

/* two elements from a cycle of length a, one from a different cycle of length b */
static uint64_t a3190_g2(uint64_t a, uint64_t b)
{
    if (a & 1) return (a - 1) / 2 * gcd_u64(a, b);
    return (a - 2) / 2 * gcd_u64(a, b) + gcd_u64(a / 2, b);
}

/* number of cycles on triples taking one element from each of three distinct cycles */
static uint64_t a3190_h(uint64_t a, uint64_t b, uint64_t c)
{
    uint64_t g = gcd_u64(a, b);
    return g * gcd_u64(a / g * b, c);
}

static int a3190_mu(uint64_t m)
{
    int r = 1;
    uint64_t p;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            m /= p;
            if (m % p == 0) return 0;
            r = -r;
        }
    }
    if (m > 1) r = -r;
    return r;
}

#define A3190_MAXN 100
#define A3190_MAXC 161701

static seq_t a3190_b[A3190_MAXN + 1];
static int a3190_have[A3190_MAXN + 1];
static seq_t a3190_cnt[A3190_MAXC];
static seq_t a3190_cc[A3190_MAXN + 1];
static uint64_t a3190_L[A3190_MAXN + 1], a3190_M[A3190_MAXN + 1];
static int a3190_big;
static unsigned __int128 a3190_tot;

static uint64_t a3190_cyc(int k)
{
    uint64_t c = 0;
    int i, j, l;
    for (i = 0; i < k; i++) {
        uint64_t Li = a3190_L[i], Mi = a3190_M[i];
        c += Mi * a3190_f3(Li);
        for (j = 0; j < k; j++)
            c += Mi * (a3190_M[j] - (i == j ? 1 : 0)) * a3190_g2(Li, a3190_L[j]);
        if (Mi >= 3) c += Mi * (Mi - 1) * (Mi - 2) / 6 * a3190_h(Li, Li, Li);
        if (Mi >= 2)
            for (j = 0; j < k; j++)
                if (j != i)
                    c += Mi * (Mi - 1) / 2 * a3190_M[j] * a3190_h(Li, Li, a3190_L[j]);
        for (j = i + 1; j < k; j++)
            for (l = j + 1; l < k; l++)
                c += Mi * a3190_M[j] * a3190_M[l] * a3190_h(Li, a3190_L[j], a3190_L[l]);
    }
    return c;
}

static void a3190_rec(uint64_t rem, uint64_t maxl, int k, seq_t coef, unsigned __int128 c128)
{
    uint64_t l;
    if (rem == 0) {
        uint64_t c = a3190_cyc(k);
        if (a3190_big) a3190_cnt[c] += coef;
        else a3190_tot += c128 << c;
        return;
    }
    for (l = (rem < maxl ? rem : maxl); l >= 1; l--) {
        seq_t cf = coef;
        unsigned __int128 cc = c128;
        uint64_t r = rem, mm, t;
        for (mm = 1; mm * l <= rem; mm++) {
            if (a3190_big) {
                for (t = 0; t < l; t++) cf *= (seq_t)(r - t);
                cf /= (seq_t)l;
                cf /= (seq_t)mm;
            } else {
                for (t = 0; t < l; t++) cc *= (unsigned __int128)(r - t);
                cc /= l;
                cc /= mm;
            }
            r -= l;
            a3190_L[k] = l;
            a3190_M[k] = mm;
            a3190_rec(r, l - 1, k + 1, cf, cc);
        }
    }
}

static seq_t a3190_polya(uint64_t m)
{
    uint64_t i;
    if (m <= 10) {
        unsigned __int128 f = 1;
        a3190_big = 0;
        a3190_tot = 0;
        a3190_rec(m, m, 0, (seq_t)1, 1);
        for (i = 2; i <= m; i++) f *= i;
        return a3190_from128(a3190_tot / f);
    } else {
        uint64_t nc = m * (m - 1) * (m - 2) / 6, c;
        seq_t S = 0, f = 1;
        a3190_big = 1;
        for (c = 0; c <= nc; c++) a3190_cnt[c] = 0;
        a3190_rec(m, m, 0, (seq_t)1, 1);
        for (c = nc + 1; c-- > 0;) {
            S += S;
            S += a3190_cnt[c];
        }
        for (i = 2; i <= m; i++) f *= (seq_t)i;
        return S / f;
    }
}

seq_t a(idx_t n)
{
    idx_t k, i, d;
    seq_t pos = 0, neg = 0;
    if (n < 1 || n > A3190_MAXN) return 0;
    for (k = 1; k <= n; k++) {
        if (!a3190_have[k]) {
            a3190_b[k] = a3190_polya(k);
            a3190_have[k] = 1;
        }
    }
    for (k = 1; k <= n; k++) {
        seq_t S = 0;
        for (i = 1; i < k; i++) S += a3190_cc[i] * a3190_b[k - i];
        a3190_cc[k] = (seq_t)k * a3190_b[k] - S;
    }
    for (d = 1; d <= n; d++) {
        if (n % d == 0) {
            int mu = a3190_mu(n / d);
            if (mu > 0) pos += a3190_cc[d];
            else if (mu < 0) neg += a3190_cc[d];
        }
    }
    return (pos - neg) / (seq_t)n;
}
