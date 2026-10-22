/*
 * A006465  Continued fraction for 4^5*Sum_{n>=0} 1/4^(2^n).
 * https://oeis.org/A006465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   a(n) = n-th partial quotient of the continued fraction of
 *            4^5*Sum_{k>=0} 1/4^(2^k). Computed exactly by running the
 *            Euclidean algorithm with big integers on the rational
 *            truncations S_N = 4^5*Sum_{k=0..N} 1/4^(2^k) = P/2^(2^(N+1)).
 *            Only the terms where S_N and S_{N+1} agree are kept
 * Range:     u8 none, u16 n <= 7881, u32 n <= 7881, u64 n <= 7881,
 *            u128 n <= 7881, big any n
 * Helpers:   mulsub_w, setbit_w, word_at
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/mulsub_w.h"
#include "../utils/setbit_w.h"
#include "../utils/word_at.h"

static long *best_cf = NULL;
static long best_len = 0;
static long *prev_cf = NULL;
static long prev_len = 0;
static int cur_N = 0;

static long bitlen_w(const uint32_t *X, size_t W) {
    size_t i = W;
    while (i > 0) {
        i--;
        if (X[i]) {
            long b = 0;
            uint32_t v = X[i];
            while (v) { b++; v >>= 1; }
            return (long)(i * 32) + b;
        }
    }
    return 0;
}

static uint64_t get64(const uint32_t *X, size_t W, long s) {
    size_t w = (size_t)(s / 32);
    int b = (int)(s % 32);
    uint64_t lo = (uint64_t)word_at(X, W, w) | ((uint64_t)word_at(X, W, w + 1) << 32);
    uint64_t hi = (uint64_t)word_at(X, W, w + 2);
    if (b == 0) return lo;
    return (lo >> b) | (hi << (64 - b));
}

static int cmp_w(const uint32_t *A, const uint32_t *B, size_t W) {
    size_t i = W;
    while (i > 0) {
        i--;
        if (A[i] != B[i]) return A[i] > B[i] ? 1 : -1;
    }
    return 0;
}

static void sub_w(uint32_t *A, const uint32_t *B, size_t W) {
    uint64_t borrow = 0;
    size_t i;
    for (i = 0; i < W; i++) {
        uint64_t ai = A[i];
        uint64_t sb = (uint64_t)B[i] + borrow;
        if (ai >= sb) { A[i] = (uint32_t)(ai - sb); borrow = 0; }
        else { A[i] = (uint32_t)(ai + 0x100000000ULL - sb); borrow = 1; }
    }
}

static long *cf_run(int N, long *len_out) {
    long D = 1L << (N + 1);
    size_t W = (size_t)((D + 10) / 32 + 4);
    uint32_t *A = (uint32_t *)calloc(W, sizeof(uint32_t));
    uint32_t *B = (uint32_t *)calloc(W, sizeof(uint32_t));
    long cap = 2 * (long)W * 32 + 16;
    long *terms = (long *)malloc((size_t)cap * sizeof(long));
    long cnt = 0;
    int k;
    for (k = 0; k <= N; k++)
        setbit_w(A, D - (1L << (k + 1)) + 10);
    setbit_w(B, D);
    while (cnt < cap) {
        long lb = bitlen_w(B, W), la, s;
        uint64_t ah, bh, q;
        uint32_t *t;
        if (lb == 0) break;
        la = bitlen_w(A, W);
        if (la - lb > 22) break;
        s = la > 63 ? la - 63 : 0;
        ah = get64(A, W, s);
        bh = get64(B, W, s);
        q = ah / (bh + 1);
        if (q) mulsub_w(A, B, q, W);
        while (cmp_w(A, B, W) >= 0) { sub_w(A, B, W); q++; }
        terms[cnt++] = (long)q;
        t = A; A = B; B = t;
    }
    free(A);
    free(B);
    *len_out = cnt;
    return terms;
}

seq_t a(idx_t n) {
    if (prev_cf == NULL) {
        cur_N = 3;
        prev_cf = cf_run(cur_N, &prev_len);
    }
    while ((idx_t)best_len <= n && cur_N < 26) {
        long nl, L = 0;
        long *nc = cf_run(cur_N + 1, &nl);
        while (L < prev_len && L < nl && prev_cf[L] == nc[L]) L++;
        if (L > best_len) {
            free(best_cf);
            best_cf = (long *)malloc((size_t)(L > 0 ? L : 1) * sizeof(long));
            memcpy(best_cf, nc, (size_t)L * sizeof(long));
            best_len = L;
        }
        free(prev_cf);
        prev_cf = nc;
        prev_len = nl;
        cur_N++;
    }
    if (n < (idx_t)best_len) return (seq_t)best_cf[(size_t)n];
    return 0;
}
