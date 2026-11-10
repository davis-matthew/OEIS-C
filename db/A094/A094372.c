/*
 * A094372  Denominators of incrementally smallest ratios A002034(n)/n.
 * https://oeis.org/A094372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = N/gcd(N, A002034(N)) where N = A094371(n) is the n-th
 *            record position of the strictly decreasing minima of
 *            A002034(N)/N. Computed by search: for each k, the divisors N of
 *            k! with A002034(N) = k are collected, then sorted and filtered
 *            for records. K is increased until the records are provab...
 * Range:     u8 n <= 24, u16 n <= 53, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef unsigned __int128 oeis_u128_t;

typedef struct {
    oeis_u128_t N;
    int k;
} A094372_cand;

static A094372_cand *A094372_recs = 0;
static int A094372_nrecs = 0;
static int A094372_safe = 0;
static int A094372_curK = 0;

static oeis_u128_t A094372_fact[40];
static int A094372_primes[20];
static int A094372_np;
static int A094372_ex[20];
static int A094372_ex1[20];
static int A094372_kk;
static A094372_cand *A094372_cands = 0;
static size_t A094372_ncand = 0, A094372_capcand = 0;

static void A094372_push(oeis_u128_t N, int k) {
    if (A094372_ncand == A094372_capcand) {
        A094372_capcand = A094372_capcand ? A094372_capcand * 2 : 1024;
        A094372_cands = (A094372_cand *)realloc(A094372_cands, A094372_capcand * sizeof(A094372_cand));
    }
    A094372_cands[A094372_ncand].N = N;
    A094372_cands[A094372_ncand].k = k;
    A094372_ncand++;
}

static void A094372_dfs(int idx, oeis_u128_t N, bool flag) {
    if (idx == A094372_np) {
        if (!flag) return;
        /* necessary condition: with j = max{j : j! < N}, need k*j! < j*N */
        int j = 1;
        while (A094372_fact[j + 1] < N) j++;
        if ((oeis_u128_t)A094372_kk * A094372_fact[j] < (oeis_u128_t)j * N)
            A094372_push(N, A094372_kk);
        return;
    }
    oeis_u128_t cur = N;
    for (int a = 0; a <= A094372_ex[idx]; a++) {
        A094372_dfs(idx + 1, cur, flag || (a > A094372_ex1[idx]));
        cur *= (oeis_u128_t)A094372_primes[idx];
    }
}

static int A094372_cmp(const void *x, const void *y) {
    oeis_u128_t a = ((const A094372_cand *)x)->N;
    oeis_u128_t b = ((const A094372_cand *)y)->N;
    return a < b ? -1 : (a > b ? 1 : 0);
}

static int A094372_legendre(int m, int p) {
    int r = 0;
    while (m) { m /= p; r += m; }
    return r;
}

static void A094372_compute(int K) {
    A094372_fact[0] = 1;
    for (int i = 1; i <= K + 1 && i < 40; i++)
        A094372_fact[i] = A094372_fact[i - 1] * (oeis_u128_t)i;
    A094372_np = 0;
    for (int p = 2; p <= K; p++) {
        bool pr = true;
        for (int d = 2; d * d <= p; d++) if (p % d == 0) { pr = false; break; }
        if (pr) A094372_primes[A094372_np++] = p;
    }
    A094372_ncand = 0;
    for (int k = 2; k <= K; k++) {
        A094372_kk = k;
        for (int i = 0; i < A094372_np; i++) {
            A094372_ex[i] = A094372_legendre(k, A094372_primes[i]);
            A094372_ex1[i] = A094372_legendre(k - 1, A094372_primes[i]);
        }
        A094372_dfs(0, 1, false);
    }
    if (A094372_ncand > 1)
        qsort(A094372_cands, A094372_ncand, sizeof(A094372_cand), A094372_cmp);

    free(A094372_recs);
    A094372_recs = (A094372_cand *)malloc((A094372_ncand + 1) * sizeof(A094372_cand));
    A094372_nrecs = 0;
    A094372_recs[A094372_nrecs].N = 1;
    A094372_recs[A094372_nrecs].k = 1;
    A094372_nrecs++;
    oeis_u128_t Nb = 1;
    int kb = 1;
    for (size_t i = 0; i < A094372_ncand; i++) {
        oeis_u128_t N = A094372_cands[i].N;
        int k = A094372_cands[i].k;
        if ((oeis_u128_t)k * Nb < (oeis_u128_t)kb * N) {
            A094372_recs[A094372_nrecs].N = N;
            A094372_recs[A094372_nrecs].k = k;
            A094372_nrecs++;
            Nb = N;
            kb = k;
        }
    }
    /* safe prefix: no record from a group > K can hide below N_i */
    A094372_safe = A094372_nrecs;
    for (int i = 1; i < A094372_nrecs; i++) {
        oeis_u128_t Np = A094372_recs[i - 1].N;
        int kp = A094372_recs[i - 1].k;
        if (A094372_recs[i].N * (oeis_u128_t)kp > (oeis_u128_t)(K + 1) * Np) {
            A094372_safe = i;
            break;
        }
    }
    A094372_curK = K;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (A094372_curK == 0 || (idx_t)A094372_safe < n) {
        int K = A094372_curK ? A094372_curK + 4 : 12;
        while (true) {
            A094372_compute(K);
            if ((idx_t)A094372_safe >= n || K >= 28) break;
            K += 4;
            if (K > 28) K = 28;
        }
    }
    if (n > (idx_t)A094372_nrecs) return 0;
    oeis_u128_t N = A094372_recs[n - 1].N;
    oeis_u128_t k = (oeis_u128_t)A094372_recs[n - 1].k;
    oeis_u128_t x = N, y = k;
    while (y) { oeis_u128_t t = x % y; x = y; y = t; }
    oeis_u128_t D = N / x;
    seq_t r = (seq_t)(uint64_t)(D >> 64);
    r = (r << 32) << 32;
    r += (seq_t)(uint64_t)D;
    return r;
}
