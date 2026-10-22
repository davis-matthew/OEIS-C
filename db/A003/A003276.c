/*
 * A003276  Numbers k such that the multiplicative group of residues prime to
 *          k, M_k, is isomorphic to M_{k+1}.
 * https://oeis.org/A003276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   search -- a segmented sieve computes phi(k); when phi(k) =
 *            phi(k+1), M_k and M_{k+1} are compared by their sorted lists of
 *            prime-power cyclic factors
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define A3276_P 120120u /* 8*3*5*7*11*13 */
#define A3276_PLIM 65536u

static uint32_t a3276_pr[6600];
static int a3276_npr = 0;
static int a3276_act = 6;
static uint64_t a3276_nx1[6600], a3276_nx2[6600];
static uint64_t *a3276_terms = 0;
static idx_t a3276_cnt = 0, a3276_cap = 0;
static uint64_t a3276_lo = 0;
static uint64_t a3276_prevphi = 0;
static uint32_t a3276_patph[A3276_P], a3276_patpr[A3276_P];
static uint32_t a3276_ph[A3276_P], a3276_pd[A3276_P];

static void a3276_init(void)
{
    uint32_t i, j;
    static const uint32_t sp[6] = {2, 3, 5, 7, 11, 13};
    static const uint32_t se[6] = {8, 3, 5, 7, 11, 13};
    char *s = (char *)calloc(A3276_PLIM + 1, 1);
    for (i = 2; i <= A3276_PLIM; i++) {
        if (!s[i]) {
            a3276_pr[a3276_npr++] = i;
            for (j = i * 2; j <= A3276_PLIM; j += i) s[j] = 1;
        }
    }
    free(s);
    for (i = 0; i < A3276_P; i++) {
        uint32_t ph = 1, pd = 1;
        int t;
        for (t = 0; t < 6; t++) {
            uint32_t p = sp[t], pe = 1;
            if (i % p) continue;
            while (pe < se[t] && i % (pe * p) == 0) pe *= p;
            ph *= pe / p * (p - 1);
            pd *= pe;
        }
        a3276_patph[i] = ph;
        a3276_patpr[i] = pd;
    }
}

static void a3276_addpp(uint64_t m, uint64_t *out, int *c)
{
    int i;
    for (i = 0; i < a3276_npr; i++) {
        uint64_t d = a3276_pr[i];
        if (d * d > m) break;
        if (m % d == 0) {
            uint64_t pw = 1;
            while (m % d == 0) { pw *= d; m /= d; }
            out[(*c)++] = pw;
        }
    }
    if (m > 1) out[(*c)++] = m;
}

static int a3276_comps(uint64_t k, uint64_t *out)
{
    int c = 0, i, j;
    for (i = 0; i < a3276_npr; i++) {
        uint64_t d = a3276_pr[i];
        if (d * d > k) break;
        if (k % d == 0) {
            int e = 0;
            uint64_t pw = 1;
            while (k % d == 0) { k /= d; e++; pw *= d; }
            if (d == 2) {
                if (e == 2) out[c++] = 2;
                else if (e >= 3) { out[c++] = 2; out[c++] = pw / 4; }
            } else {
                a3276_addpp(d - 1, out, &c);
                if (e >= 2) out[c++] = pw / d;
            }
        }
    }
    if (k > 1 && k != 2) a3276_addpp(k - 1, out, &c);
    for (i = 1; i < c; i++) {
        uint64_t t = out[i];
        j = i - 1;
        while (j >= 0 && out[j] > t) { out[j + 1] = out[j]; j--; }
        out[j + 1] = t;
    }
    return c;
}

static bool a3276_iso(uint64_t k1, uint64_t k2)
{
    uint64_t e1[128], e2[128];
    int l1 = a3276_comps(k1, e1), l2 = a3276_comps(k2, e2), i;
    if (l1 != l2) return false;
    for (i = 0; i < l1; i++)
        if (e1[i] != e2[i]) return false;
    return true;
}

static void a3276_push(uint64_t k)
{
    if (a3276_cnt == a3276_cap) {
        a3276_cap = a3276_cap ? a3276_cap * 2 : 64;
        a3276_terms = (uint64_t *)realloc(a3276_terms, sizeof(uint64_t) * a3276_cap);
    }
    a3276_terms[a3276_cnt++] = k;
}

static void a3276_hits(uint64_t lo, uint64_t hi, uint64_t pe, uint32_t f, uint32_t g)
{
    uint64_t m = ((lo + pe - 1) / pe) * pe;
    for (; m < hi; m += pe) {
        uint32_t idx = (uint32_t)(m - lo);
        a3276_ph[idx] *= f;
        a3276_pd[idx] *= g;
    }
}

static void a3276_segment(void)
{
    uint64_t lo = a3276_lo, hi = lo + A3276_P;
    uint32_t i;
    int j;
    static const uint32_t sp[6] = {2, 3, 5, 7, 11, 13};
    static const uint32_t sx[6] = {16, 9, 25, 49, 121, 169};

    memcpy(a3276_ph, a3276_patph, sizeof(a3276_ph));
    memcpy(a3276_pd, a3276_patpr, sizeof(a3276_pd));

    for (j = 0; j < 6; j++) {
        uint64_t pe;
        for (pe = sx[j]; pe < hi; pe *= sp[j])
            a3276_hits(lo, hi, pe, sp[j], sp[j]);
    }

    while (a3276_act < a3276_npr &&
           (uint64_t)a3276_pr[a3276_act] * a3276_pr[a3276_act] < hi) {
        uint64_t p = a3276_pr[a3276_act];
        a3276_nx1[a3276_act] = ((lo + p - 1) / p) * p;
        a3276_nx2[a3276_act] = ((lo + p * p - 1) / (p * p)) * (p * p);
        a3276_act++;
    }

    for (j = 6; j < a3276_act; j++) {
        uint32_t p = a3276_pr[j];
        uint64_t p2 = (uint64_t)p * p, m, pe;
        for (m = a3276_nx1[j]; m < hi; m += p) {
            uint32_t idx = (uint32_t)(m - lo);
            a3276_ph[idx] *= p - 1;
            a3276_pd[idx] *= p;
        }
        a3276_nx1[j] = m;
        for (m = a3276_nx2[j]; m < hi; m += p2) {
            uint32_t idx = (uint32_t)(m - lo);
            a3276_ph[idx] *= p;
            a3276_pd[idx] *= p;
        }
        a3276_nx2[j] = m;
        for (pe = p2 * p; pe < hi; pe *= p)
            a3276_hits(lo, hi, pe, p, p);
    }

    for (i = 0; i < A3276_P; i++) {
        uint64_t k = lo + i, q, phi;
        if (k == 0) { a3276_prevphi = 0; continue; }
        q = (uint64_t)((double)k / (double)a3276_pd[i]);
        phi = q > 1 ? (uint64_t)a3276_ph[i] * (q - 1) : (uint64_t)a3276_ph[i];
        if (k > 1 && phi == a3276_prevphi && a3276_iso(k - 1, k))
            a3276_push(k - 1);
        a3276_prevphi = phi;
    }
    a3276_lo = hi;
}

seq_t a(idx_t n)
{
    if (a3276_npr == 0) a3276_init();
    while (a3276_cnt < n) a3276_segment();
    return (seq_t)a3276_terms[n - 1];
}
