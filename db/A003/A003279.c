/*
 * A003279  High temperature series for spherical model susceptibility on
 *          3-dimensional simple cubic lattice.
 * https://oeis.org/A003279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   chi = 1/(P(y)*(1-6y)) with x = y*P(y), where P(t) = Sum_i
 *            A002899(i)*t^(2i) (closed walks on the simple cubic lattice). By
 *            Lagrange inversion: a(n) = [t^n] (P(t)+t*P'(t)) * P(t)^(-(n+2))
 *            / (1-6t). A002899: i^3 b(i) = 2(2i-1)(10i^2-10i+3) b(i-1) -
 *            36(i-1)(2i-1)(2i-3) b(i-2), b(0)=1, b(1)=6
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static void a3279_fin(seq_t pos, seq_t ng, seq_t *m, bool *s) {
    if (pos >= ng) { *m = pos - ng; *s = false; }
    else { *m = ng - pos; *s = true; }
}

static int a3279_isfixed(void) {
    seq_t x = 1;
    int i;
    for (i = 0; i < 300; i++) x = x * 2;
    return x == 0;
}

/* c = a*b truncated to L terms, sign-magnitude; c must not alias a or b */
static void a3279_smul(const seq_t *am, const bool *as, const seq_t *bm, const bool *bs,
                       seq_t *cm, bool *cs, idx_t L) {
    idx_t k, j;
    for (k = 0; k < L; k++) {
        seq_t pos = 0, ng = 0;
        for (j = 0; j <= k; j++) {
            if (am[j] == 0 || bm[k - j] == 0) continue;
            seq_t v = am[j] * bm[k - j];
            if (as[j] != bs[k - j]) ng += v; else pos += v;
        }
        a3279_fin(pos, ng, &cm[k], &cs[k]);
    }
}

seq_t a(idx_t n) {
    idx_t M = n / 2, L = M + 1, i, j, k;
    int fixedw = a3279_isfixed();
    seq_t *p = mem_new(seq_t, L);
    seq_t *Fm = mem_new(seq_t, L);
    bool *Fs = (bool *)calloc(L, sizeof(bool));

    if (!fixedw) {
        p[0] = 1;
        if (M >= 1) p[1] = 6;
        for (i = 2; i <= M; i++) {
            idx_t c1 = 2 * (2 * i - 1) * (10 * i * i - 10 * i + 3);
            idx_t c2 = 36 * (i - 1) * (2 * i - 1) * (2 * i - 3);
            seq_t t = p[i - 1] * c1 - p[i - 2] * c2;
            p[i] = t / (i * i * i);
        }
        /* F = P^(-(n+2)) via power recurrence (exact division) */
        Fm[0] = 1; Fs[0] = false;
        for (k = 1; k < L; k++) {
            seq_t pos = 0, ng = 0;
            for (j = 1; j <= k; j++) {
                if (Fm[k - j] == 0) continue;
                idx_t c = (n + 1) * j + k;
                seq_t v = p[j] * Fm[k - j] * c;
                if (Fs[k - j]) pos += v; else ng += v;
            }
            a3279_fin(pos, ng, &Fm[k], &Fs[k]);
            Fm[k] = Fm[k] / k;
        }
    } else {
        /* exact modulo 2^w: binomials via Pascal additions only */
        seq_t *row = mem_new(seq_t, 2 * M + 1);
        seq_t *cen = mem_new(seq_t, L);
        idx_t r;
        row[0] = 1; cen[0] = 1;
        for (r = 1; r <= 2 * M; r++) {
            for (j = r; j >= 1; j--) row[j] += row[j - 1];
            if (r % 2 == 0) cen[r / 2] = row[r / 2];
        }
        for (j = 0; j <= 2 * M; j++) row[j] = 0;
        row[0] = 1;
        for (i = 0; i <= M; i++) {
            seq_t sum = 0;
            for (k = 0; k <= i; k++) sum += row[k] * row[k] * cen[k];
            p[i] = cen[i] * sum;
            if (i < M)
                for (j = i + 1; j >= 1; j--) row[j] += row[j - 1];
        }
        mem_free(row);
        mem_free(cen);

        /* Q = 1/P */
        seq_t *Qm = mem_new(seq_t, L);
        bool *Qs = (bool *)calloc(L, sizeof(bool));
        seq_t *Tm = mem_new(seq_t, L);
        bool *Ts = (bool *)calloc(L, sizeof(bool));
        Qm[0] = 1;
        for (k = 1; k < L; k++) {
            seq_t pos = 0, ng = 0;
            for (j = 1; j <= k; j++) {
                seq_t v = p[j] * Qm[k - j];
                if (Qs[k - j]) pos += v; else ng += v;
            }
            a3279_fin(pos, ng, &Qm[k], &Qs[k]);
        }
        /* F = Q^(n+2) by binary exponentiation */
        Fm[0] = 1;
        idx_t e = n + 2;
        while (e) {
            if (e & 1) {
                a3279_smul(Fm, Fs, Qm, Qs, Tm, Ts, L);
                for (j = 0; j < L; j++) { Fm[j] = Tm[j]; Fs[j] = Ts[j]; }
            }
            e >>= 1;
            if (e) {
                a3279_smul(Qm, Qs, Qm, Qs, Tm, Ts, L);
                for (j = 0; j < L; j++) { Qm[j] = Tm[j]; Qs[j] = Ts[j]; }
            }
        }
        mem_free(Qm); free(Qs); mem_free(Tm); free(Ts);
    }

    /* G = D * F, D_i = (2i+1) p_i; answer = Sum_i G_i 6^(n-2i) */
    seq_t apos = 0, aneg = 0;
    seq_t pw = (n - 2 * M) ? (seq_t)6 : (seq_t)1;
    for (i = L; i-- > 0;) {
        seq_t pos = 0, ng = 0, gm;
        bool gs;
        for (j = 0; j <= i; j++) {
            if (Fm[i - j] == 0) continue;
            seq_t v = p[j] * (2 * j + 1) * Fm[i - j];
            if (Fs[i - j]) ng += v; else pos += v;
        }
        a3279_fin(pos, ng, &gm, &gs);
        if (gs) aneg += gm * pw; else apos += gm * pw;
        pw *= 36;
    }
    mem_free(p); mem_free(Fm); free(Fs);
    return apos - aneg;
}
