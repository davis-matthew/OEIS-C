/*
 * A001323  Number of equivalence classes of 3-valued Post functions of n
 *          variables under action of semi-direct product of symmetric group
 *          S_n and complementing group C(n,3).
 * https://oeis.org/A001323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside's lemma over G = (Z_3)^n x| S_n acting on {0,1,2}^n:
 *            a(n) = (1/(3^n n!)) * Sum_{g in G} 3^{cyc(g)}, where cyc(g) is
 *            the number of cycles of x -> P x + c. A k-cycle of P with shift
 *            sum s contributes 3^gcd(j,k) fixed points to g^j when s = 0.
 *            When s != 0 it contributes 3^(gcd(j,3k)/3) if v...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   a1323_add, a1323_cyc, a1323_div, a1323_gcd, a1323_mul,
 *            a1323_mulpow3, a1323_phi, a1323_v3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a1323_add.h"
#include "../utils/a1323_cyc.h"
#include "../utils/a1323_div.h"
#include "../utils/a1323_gcd.h"
#include "../utils/a1323_mul.h"
#include "../utils/a1323_mulpow3.h"
#include "../utils/a1323_phi.h"
#include "../utils/a1323_v3.h"

typedef struct { uint64_t e; uint32_t *c; size_t cl; } a1323_term;

static a1323_term *a1323_terms = NULL;
static size_t a1323_nt = 0, a1323_tcap = 0;
static idx_t a1323_n;
static int a1323_mult[64];

static void a1323_push(uint64_t e, uint32_t *c, size_t cl)
{
    if (a1323_nt == a1323_tcap) {
        a1323_tcap = a1323_tcap ? 2 * a1323_tcap : 64;
        a1323_terms = (a1323_term *)realloc(a1323_terms, a1323_tcap * sizeof(a1323_term));
    }
    a1323_terms[a1323_nt].e = e;
    a1323_terms[a1323_nt].c = c;
    a1323_terms[a1323_nt].cl = cl;
    a1323_nt++;
}

static void a1323_part(void)
{
    idx_t n = a1323_n, k;
    uint64_t ks[64];
    int ms[64], ts[64];
    int np = 0, m = 0, i, r;
    size_t cap = (size_t)n + 8, cl = 1;
    uint32_t *cls;
    for (k = 1; k <= n; k++)
        if (a1323_mult[k] > 0) {
            ks[np] = k; ms[np] = a1323_mult[k]; ts[np] = 0; np++;
            m += a1323_mult[k];
        }
    cls = (uint32_t *)calloc(cap, sizeof(uint32_t));
    cls[0] = 1;
    for (k = 2; k <= n; k++) cl = a1323_mul(cls, cl, (uint32_t)k);
    for (i = 0; i < np; i++) {
        for (r = 0; r < ms[i]; r++) a1323_div(cls, cl, (uint32_t)ks[i]);
        for (r = 2; r <= ms[i]; r++) a1323_div(cls, cl, (uint32_t)r);
    }
    while (cl > 1 && cls[cl - 1] == 0) cl--;
    for (;;) {
        uint64_t e = a1323_cyc(np, ks, ms, ts);
        uint32_t *c = (uint32_t *)calloc(cap, sizeof(uint32_t));
        size_t L = cl;
        int T = 0, j;
        memcpy(c, cls, cl * sizeof(uint32_t));
        for (r = 0; r < (int)n - m; r++) L = a1323_mul(c, L, 3);
        for (i = 0; i < np; i++) {
            T += ts[i];
            for (j = 1; j <= ts[i]; j++) {
                L = a1323_mul(c, L, (uint32_t)(ms[i] - ts[i] + j));
                a1323_div(c, L, (uint32_t)j);
            }
        }
        for (r = 0; r < T; r++) L = a1323_mul(c, L, 2);
        while (L > 1 && c[L - 1] == 0) L--;
        a1323_push(e, c, L);
        i = 0;
        while (i < np && ts[i] == ms[i]) { ts[i] = 0; i++; }
        if (i == np) break;
        ts[i]++;
    }
    free(cls);
}

static void a1323_rec(idx_t k, idx_t rem)
{
    idx_t c;
    if (rem == 0) {
        idx_t j;
        for (j = 1; j <= k; j++) a1323_mult[j] = 0;
        a1323_part();
        return;
    }
    if (k == 0) return;
    for (c = rem / k;; c--) {
        a1323_mult[k] = (int)c;
        a1323_rec(k - 1, rem - c * k);
        if (c == 0) break;
    }
}

static int a1323_cmp(const void *p, const void *q)
{
    uint64_t x = ((const a1323_term *)p)->e, y = ((const a1323_term *)q)->e;
    return x < y ? 1 : (x > y ? -1 : 0);
}

seq_t a(idx_t n)
{
    uint64_t emax, prev;
    size_t cap, len = 1, t;
    uint32_t *acc;
    idx_t j;
    seq_t r = 0;
    if (n < 1 || n > 30) return 0;
    a1323_n = n;
    a1323_nt = 0;
    a1323_rec(n, n);
    qsort(a1323_terms, a1323_nt, sizeof(a1323_term), a1323_cmp);
    emax = a1323_terms[0].e;
    cap = (size_t)(emax * 1585 / 32000) + (size_t)n * 2 + 32;
    acc = (uint32_t *)calloc(cap, sizeof(uint32_t));
    prev = emax;
    for (t = 0; t < a1323_nt; t++) {
        len = a1323_mulpow3(acc, len, prev - a1323_terms[t].e);
        len = a1323_add(acc, len, a1323_terms[t].c, a1323_terms[t].cl);
        prev = a1323_terms[t].e;
        free(a1323_terms[t].c);
    }
    len = a1323_mulpow3(acc, len, prev);
    for (j = 0; j < n; j++) a1323_div(acc, len, 3);
    for (j = 2; j <= n; j++) a1323_div(acc, len, (uint32_t)j);
    while (len > 1 && acc[len - 1] == 0) len--;
    for (t = len; t-- > 0;) {
        int sh;
        for (sh = 24; sh >= 0; sh -= 8) {
            r = (seq_t)(r << 8);
            r += (idx_t)((acc[t] >> sh) & 255u);
        }
    }
    free(acc);
    return r;
}
