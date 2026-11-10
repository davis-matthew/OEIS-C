/*
 * A220891  Where record values occur in A187824.
 * https://oeis.org/A220891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least x >= 2 with A187824(x) > A187824(a(n-1)), a(1) = 2.
 *            Found by a CRT search: x mod k must lie in {0, 1, k-1} for every
 *            k <= m. The search goes through prime powers in increasing order
 *            and prunes with the composite moduli and with the best value
 *            found so far
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 18, u64 n <= 25, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 30 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

#define A220891_MAXM 1024

static int A220891_qs[A220891_MAXM];
static int A220891_cnt;
static int A220891_kl[A220891_MAXM];
static int A220891_ks[A220891_MAXM + 1];
static seq_t A220891_best;
static bool A220891_found;

/* A187824(x): largest m with x mod k in {0,1,k-1} for all k = 1..m */
static int A220891_g(seq_t x) {
    int m = 0;
    for (int k = 1;; k++) {
        int r = (int)(x % (seq_t)k);
        if (r == 0 || r == 1 || r == k - 1) m++;
        else break;
    }
    return m;
}

static void A220891_dfs(int i, seq_t x, seq_t M, bool movf) {
    if (A220891_found && !(x < A220891_best)) return;
    if (i == A220891_cnt) {
        seq_t xv = x;
        if (xv < 2) {
            if (movf) return;
            xv += M;
            if (xv < M) return; /* wrapped */
        }
        if (A220891_found && !(xv < A220891_best)) return;
        A220891_best = xv;
        A220891_found = true;
        return;
    }
    int q = A220891_qs[i];
    seq_t nM = M * (seq_t)q;
    bool novf = movf || (nM / (seq_t)q != M);
    for (int c = 0; c < 3; c++) {
        int r = (c == 0) ? 0 : (c == 1 ? 1 : q - 1);
        if (c == 2 && q - 1 == 1) continue;
        seq_t nx = x;
        bool ok = false;
        for (int t = 0; t < q; t++) {
            if ((int)(nx % (seq_t)q) == r) { ok = true; break; }
            if (movf) break;
            seq_t old = nx;
            nx += M;
            if (nx < old) break; /* wrapped */
        }
        if (!ok) continue;
        if (A220891_found && !(nx < A220891_best)) continue;
        /* check all k <= m whose largest prime factor is this prime */
        bool good = true;
        for (int j = A220891_ks[i]; j < A220891_ks[i + 1]; j++) {
            int k = A220891_kl[j];
            int rr = (int)(nx % (seq_t)k);
            if (!(rr == 0 || rr == 1 || rr == k - 1)) { good = false; break; }
        }
        if (good) A220891_dfs(i + 1, nx, nM, novf);
    }
}

/* least x >= 2 with A187824(x) >= m; returns false if not representable */
static bool A220891_f(int m, seq_t *out) {
    if (m >= A220891_MAXM) return false;
    static int lpf[A220891_MAXM];
    static int pidx[A220891_MAXM];
    int primes[A220891_MAXM];
    int np = 0;
    for (int k = 0; k <= m; k++) lpf[k] = 0;
    for (int p = 2; p <= m; p++) {
        if (lpf[p] != 0) continue;
        /* p is prime: it becomes the largest prime factor of its multiples */
        for (int k = p; k <= m; k += p) lpf[k] = p;
        pidx[p] = np;
        primes[np++] = p;
    }
    A220891_cnt = np;
    for (int i = 0; i < np; i++) {
        int p = primes[i];
        int q = p;
        while ((long long)q * p <= m) q *= p;
        A220891_qs[i] = q;
    }
    /* group k = 2..m by largest prime factor (increasing primes) */
    int pos = 0;
    for (int i = 0; i < np; i++) {
        A220891_ks[i] = pos;
        int p = primes[i];
        for (int k = 2; k <= m; k++)
            if (lpf[k] == p) A220891_kl[pos++] = k;
    }
    A220891_ks[np] = pos;
    (void)pidx;
    A220891_found = false;
    A220891_best = 0;
    A220891_dfs(0, (seq_t)0, (seq_t)1, false);
    if (A220891_found) *out = A220891_best;
    return A220891_found;
}

static seq_t *A220891_recs = 0;
static int A220891_nrec = 0;
static int A220891_cap = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (!A220891_recs) {
        A220891_cap = 64;
        A220891_recs = mem_new(seq_t, A220891_cap);
        A220891_recs[0] = 2;
        A220891_nrec = 1;
    }
    while ((idx_t)A220891_nrec < n) {
        seq_t x = A220891_recs[A220891_nrec - 1];
        int m = A220891_g(x) + 1;
        seq_t y;
        if (!A220891_f(m, &y)) return 0;
        if (A220891_nrec >= A220891_cap) {
            A220891_cap *= 2;
            A220891_recs = mem_resize(seq_t, A220891_recs, A220891_cap);
        }
        A220891_recs[A220891_nrec++] = y;
    }
    return A220891_recs[n - 1];
}
