/*
 * A098653  Where A007535 reaches a record.
 * https://oeis.org/A098653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   no closed form; computed by a search. Let A007535(b) be the
 *            smallest composite m > b with b^(m-1) == 1 (mod m). a(n) is the
 *            n-th b at which A007535(b) is larger than every earlier value (a
 *            record). Each composite m is taken in increasing order. Its
 *            Fermat liars b < m (a subgroup of (Z/mZ)^* of si...
 * Range:     u8 n <= 6, u16 n <= 62, u32 n <= 432, u64 n <= 432,
 *            u128 n <= 432, big any n
 * Validated: standard, pass, 60 terms through n = 388 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *A098653_spf = NULL;
static uint64_t A098653_L = 0;
static uint32_t *A098653_ans = NULL;     /* ans[b] = A007535(b), 0 = unknown */
static uint32_t *A098653_U = NULL;       /* unresolved b's (lazy) */
static uint64_t A098653_Usz = 0, A098653_Ucap = 0, A098653_ulive = 0;
static uint64_t *A098653_rec = NULL;
static uint64_t A098653_rcnt = 0, A098653_rcap = 0;
static uint64_t A098653_m = 2, A098653_lo = 1, A098653_rmax = 0;
static uint64_t *A098653_R = NULL, *A098653_T = NULL, *A098653_S = NULL;
static uint64_t A098653_Rcap = 0, A098653_Scap = 0;

static void A098653_extend(uint64_t nl) {
    uint64_t i, j;
    if (A098653_spf) free(A098653_spf);
    A098653_spf = (uint32_t *)calloc(nl + 1, sizeof(uint32_t));
    for (i = 2; i <= nl; i++) {
        if (A098653_spf[i]) continue;
        A098653_spf[i] = (uint32_t)i;
        for (j = i * i; j <= nl; j += i)
            if (!A098653_spf[j]) A098653_spf[j] = (uint32_t)i;
    }
    A098653_ans = (uint32_t *)realloc(A098653_ans, (nl + 1) * sizeof(uint32_t));
    for (i = (A098653_L ? A098653_L + 1 : 0); i <= nl; i++) A098653_ans[i] = 0;
    A098653_L = nl;
}

static uint64_t A098653_pm(uint64_t b, uint64_t e, uint64_t q) {
    uint64_t r = 1 % q;
    b %= q;
    while (e) {
        if (e & 1) r = r * b % q;
        b = b * b % q;
        e >>= 1;
    }
    return r;
}

static uint64_t A098653_inv(uint64_t a, uint64_t q) {
    int64_t t = 0, nt = 1, r = (int64_t)q, nr = (int64_t)(a % q), x;
    while (nr) {
        int64_t qq = r / nr;
        x = t - qq * nt; t = nt; nt = x;
        x = r - qq * nr; r = nr; nr = x;
    }
    if (t < 0) t += (int64_t)q;
    return (uint64_t)t;
}

static void A098653_ensureR(uint64_t need) {
    if (need > A098653_Rcap) {
        while (A098653_Rcap < need) A098653_Rcap = A098653_Rcap ? 2 * A098653_Rcap : 1024;
        A098653_R = (uint64_t *)realloc(A098653_R, A098653_Rcap * sizeof(uint64_t));
        A098653_T = (uint64_t *)realloc(A098653_T, A098653_Rcap * sizeof(uint64_t));
    }
}

/* fill S with the subgroup of order g in (Z/qZ)^*, q = p^e */
static void A098653_subgroup(uint64_t p, uint64_t q, uint64_t g) {
    uint64_t phi = (p - 1) * (q / p), h = 1, c, s, j;
    uint64_t fp[64];
    int nf = 0, i;
    if (g > A098653_Scap) {
        while (A098653_Scap < g) A098653_Scap = A098653_Scap ? 2 * A098653_Scap : 1024;
        A098653_S = (uint64_t *)realloc(A098653_S, A098653_Scap * sizeof(uint64_t));
    }
    if (g == 1) { A098653_S[0] = 1; return; }
    if (g == 2) { A098653_S[0] = 1; A098653_S[1] = q - 1; return; }
    s = g;
    while (s > 1) {
        uint64_t r = A098653_spf[s];
        fp[nf++] = r;
        while (s % r == 0) s /= r;
    }
    for (c = 2; c < q; c++) {
        bool ok = true;
        if (c % p == 0) continue;
        h = A098653_pm(c, phi / g, q);
        for (i = 0; i < nf; i++)
            if (A098653_pm(h, g / fp[i], q) == 1) { ok = false; break; }
        if (ok) break;
    }
    s = 1;
    for (j = 0; j < g; j++) { A098653_S[j] = s; s = s * h % q; }
}

static void A098653_step(void) {
    uint64_t m = A098653_m, k, o, t, F = 1;
    uint64_t pp[32], pq[32], pg[32];
    int np = 0, i;
    if (m > A098653_L) A098653_extend(A098653_L ? 2 * A098653_L : 65536);
    /* b = m-1 becomes a candidate */
    if (A098653_Usz >= A098653_Ucap) {
        A098653_Ucap = A098653_Ucap ? 2 * A098653_Ucap : 1024;
        A098653_U = (uint32_t *)realloc(A098653_U, A098653_Ucap * sizeof(uint32_t));
    }
    A098653_U[A098653_Usz++] = (uint32_t)(m - 1);
    A098653_ulive++;

    if (A098653_spf[m] != m) {
        k = (uint64_t)__builtin_ctzll(m);
        o = m >> k;
        t = o;
        while (t > 1) {
            uint64_t p = A098653_spf[t], q = 1, a = m - 1, bq = p - 1, g;
            while (t % p == 0) { t /= p; q *= p; }
            while (bq) { uint64_t r = a % bq; a = bq; bq = r; }
            g = a;
            pp[np] = p; pq[np] = q; pg[np] = g; np++;
            F *= g;
        }
        if (F <= 40 * A098653_ulive + 40) {
            /* enumerate liars by CRT */
            uint64_t cnt = 1, M = (uint64_t)1 << k, x;
            A098653_ensureR(F + 1);
            A098653_R[0] = (k > 0) ? 1 : 0;
            for (i = 0; i < np; i++) {
                uint64_t q = pq[i], g = pg[i], invM = A098653_inv(M % q, q);
                uint64_t nc = 0, r, j;
                A098653_subgroup(pp[i], q, g);
                for (r = 0; r < cnt; r++) {
                    uint64_t rv = A098653_R[r], rm = rv % q;
                    for (j = 0; j < g; j++) {
                        uint64_t d = (A098653_S[j] + q - rm) % q;
                        A098653_T[nc++] = rv + M * (d * invM % q);
                    }
                }
                { uint64_t *tmp = A098653_R; A098653_R = A098653_T; A098653_T = tmp; }
                cnt = nc;
                M *= q;
            }
            for (x = 0; x < cnt; x++) {
                uint64_t b = A098653_R[x];
                if (b >= 1 && b < m && A098653_ans[b] == 0) {
                    A098653_ans[b] = (uint32_t)m;
                    A098653_ulive--;
                }
            }
            if (A098653_Usz > 2 * A098653_ulive + 1024) {
                uint64_t w = 0, r;
                for (r = 0; r < A098653_Usz; r++)
                    if (A098653_ans[A098653_U[r]] == 0) A098653_U[w++] = A098653_U[r];
                A098653_Usz = w;
            }
        } else {
            /* test each unresolved b directly */
            uint64_t w = 0, r, mask = ((uint64_t)1 << k) - 1;
            for (r = 0; r < A098653_Usz; r++) {
                uint64_t b = A098653_U[r];
                bool liar;
                if (A098653_ans[b]) continue;
                if (k == 0) liar = A098653_pm(b, m - 1, m) == 1;
                else liar = (b & mask) == 1 && (o == 1 || A098653_pm(b, m - 1, o) == 1);
                if (liar) { A098653_ans[b] = (uint32_t)m; A098653_ulive--; }
                else A098653_U[w++] = (uint32_t)b;
            }
            A098653_Usz = w;
        }
    }
    /* advance records over fully resolved prefix */
    while (A098653_lo < m && A098653_ans[A098653_lo] != 0) {
        uint64_t v = A098653_ans[A098653_lo];
        if (v > A098653_rmax) {
            A098653_rmax = v;
            if (A098653_rcnt >= A098653_rcap) {
                A098653_rcap = A098653_rcap ? 2 * A098653_rcap : 256;
                A098653_rec = (uint64_t *)realloc(A098653_rec, A098653_rcap * sizeof(uint64_t));
            }
            A098653_rec[A098653_rcnt++] = A098653_lo;
        }
        A098653_lo++;
    }
    A098653_m = m + 1;
}

seq_t a(idx_t n) {
    while (A098653_rcnt < n) A098653_step();
    return (seq_t)A098653_rec[n - 1];
}
