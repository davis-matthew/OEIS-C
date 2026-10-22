/*
 * A003283  Denominators of coefficients of Green function for cubic lattice.
 * https://oeis.org/A003283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   C(n) rational with 8*(n+1)*(2n+1)*(2n+3)*C(n+1) =
 *            6*(2n+1)*(5n^2+5n+2)*C(n) - 24*n^3*C(n-1) -
 *            2*n*(n-1)*(2n-1)*C(n-2), C(0) = 1, C(n) = 0 for n < 0; a(n) =
 *            denominator(C(n)). Computed as E(n) = C(n)*L(n), L(n) =
 *            prod_{k=0..n-1} 8(k+1)(2k+1)(2k+3), with exact integer
 *            recurrence, then a(n) = L(n)/g...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   z3283_addf, z3283_am, z3283_bl, z3283_divP, z3283_m, z3283_mod,
 *            z3283_ms, z3283_sb, z3283_trim
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/z3283_addf.h"
#include "../utils/z3283_am.h"
#include "../utils/z3283_bl.h"
#include "../utils/z3283_divP.h"
#include "../utils/z3283_m.h"
#include "../utils/z3283_mod.h"
#include "../utils/z3283_ms.h"
#include "../utils/z3283_sb.h"
#include "../utils/z3283_trim.h"

static uint64_t *z3283_E[3];
static size_t z3283_len[3];
static uint64_t z3283_B[3];
static uint64_t *z3283_tmp = NULL;
static size_t z3283_W = 0;
static idx_t z3283_n = 0;
static int z3283_init_done = 0;

static void z3283_reset(void) {
    int i;
    if (z3283_init_done) {
        for (i = 0; i < 3; i++) free(z3283_E[i]);
        free(z3283_tmp);
    }
    z3283_W = 16;
    for (i = 0; i < 3; i++) {
        z3283_E[i] = (uint64_t *)calloc(z3283_W, sizeof(uint64_t));
        z3283_len[i] = 1;
        z3283_B[i] = 1;
    }
    z3283_tmp = (uint64_t *)calloc(z3283_W, sizeof(uint64_t));
    z3283_E[2][0] = 1;
    z3283_B[2] = 2;
    z3283_n = 0;
    z3283_init_done = 1;
}

static void z3283_ensure(size_t L) {
    int i;
    size_t nw;
    if (L <= z3283_W) return;
    nw = z3283_W * 2;
    if (nw < L) nw = L;
    for (i = 0; i < 3; i++)
        z3283_E[i] = (uint64_t *)realloc(z3283_E[i], nw * sizeof(uint64_t));
    z3283_tmp = (uint64_t *)realloc(z3283_tmp, nw * sizeof(uint64_t));
    z3283_W = nw;
}

static void z3283_extend(int i, size_t L) {
    size_t j, len = z3283_len[i];
    uint64_t s = (len > 0 && (z3283_E[i][len - 1] >> 63)) ? ~(uint64_t)0 : 0;
    for (j = len; j < L; j++) z3283_E[i][j] = s;
    if (L > len) z3283_len[i] = L;
}

static void z3283_step(void) {
    uint64_t n = (uint64_t)z3283_n;
    uint64_t ca = 6 * (2 * n + 1) * (5 * n * n + 5 * n + 2);
    uint64_t m1 = n >= 1 ? z3283_m(n - 1) : 0;
    uint64_t cb = 24 * n * n * n;
    uint64_t cc = n >= 2 ? 2 * n * (n - 1) * (2 * n - 1) : 0;
    uint64_t m2 = n >= 2 ? z3283_m(n - 2) : 0;
    uint64_t t1 = (uint64_t)z3283_bl(ca) + z3283_B[2];
    uint64_t t2 = (uint64_t)z3283_bl(m1) + z3283_bl(cb) + z3283_B[1];
    uint64_t t3 = (uint64_t)z3283_bl(m1) + z3283_bl(cc) + z3283_bl(m2) + z3283_B[0];
    uint64_t nb = t1;
    size_t L;
    int i;
    uint64_t *e0, *e1, *e2;
    if (t2 > nb) nb = t2;
    if (t3 > nb) nb = t3;
    nb += 2;
    L = (size_t)(nb / 64 + 2);
    z3283_ensure(L);
    for (i = 0; i < 3; i++) z3283_extend(i, L);
    e0 = z3283_E[0]; e1 = z3283_E[1]; e2 = z3283_E[2];
    z3283_ms(z3283_tmp, e0, L, m2);
    z3283_ms(z3283_tmp, z3283_tmp, L, cc);
    z3283_am(z3283_tmp, e1, L, cb);
    z3283_ms(z3283_tmp, z3283_tmp, L, m1);
    z3283_ms(e0, e2, L, ca);
    z3283_sb(e0, z3283_tmp, L);
    z3283_E[0] = e1; z3283_E[1] = e2; z3283_E[2] = e0;
    z3283_len[0] = L; z3283_len[1] = L; z3283_len[2] = L;
    z3283_B[0] = z3283_B[1]; z3283_B[1] = z3283_B[2]; z3283_B[2] = nb;
    z3283_n++;
}

seq_t a(idx_t n) {
    size_t len, i;
    uint64_t *w;
    uint64_t M, p, k;
    uint32_t *spf;
    uint64_t *vL;
    seq_t res;
    uint64_t chunk;

    if (!z3283_init_done || n < z3283_n) z3283_reset();
    while (z3283_n < n) z3283_step();

    len = z3283_len[2];
    w = (uint64_t *)malloc((len + 1) * sizeof(uint64_t));
    memcpy(w, z3283_E[2], len * sizeof(uint64_t));
    if (w[len - 1] >> 63) {
        uint64_t c = 1;
        for (i = 0; i < len; i++) {
            w[i] = ~w[i] + c;
            c = (c && w[i] == 0) ? 1 : 0;
        }
    }
    len = z3283_trim(w, len);
    if (len == 0) { free(w); return (seq_t)1; }

    M = 2 * (uint64_t)n + 3;
    spf = (uint32_t *)calloc(M + 1, sizeof(uint32_t));
    for (p = 2; p <= M; p++) {
        if (spf[p] == 0) {
            uint64_t q;
            for (q = p; q <= M; q += p)
                if (spf[q] == 0) spf[q] = (uint32_t)p;
        }
    }
    vL = (uint64_t *)calloc(M + 1, sizeof(uint64_t));
    vL[2] += 3 * (uint64_t)n;
    for (k = 0; k < (uint64_t)n; k++) {
        z3283_addf(k + 1, spf, vL);
        z3283_addf(2 * k + 1, spf, vL);
        z3283_addf(2 * k + 3, spf, vL);
    }

    res = 1;
    chunk = 1;
    for (p = 2; p <= M; p++) {
        uint64_t need = vL[p], got = 0, ex;
        if (need == 0) continue;
        if (p == 2) {
            size_t j = 0;
            while (j < len && w[j] == 0) { got += 64; j++; }
            if (j < len) {
                uint64_t x = w[j];
                while (!(x & 1)) { got++; x >>= 1; }
            }
        } else {
            uint64_t P = p, e = 1;
            while (P <= UINT64_MAX / p) { P *= p; e++; }
            while (got < need && len > 0) {
                uint64_t r = z3283_mod(w, len, P);
                if (r == 0) {
                    len = z3283_divP(w, len, P);
                    got += e;
                } else {
                    while (r % p == 0) { r /= p; got++; }
                    break;
                }
            }
        }
        if (got > need) got = need;
        for (ex = need - got; ex > 0; ex--) {
            if (chunk > UINT64_MAX / p) {
                res *= (seq_t)chunk;
                chunk = 1;
            }
            chunk *= p;
        }
    }
    res *= (seq_t)chunk;

    free(w);
    free(spf);
    free(vL);
    return res;
}
