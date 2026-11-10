/*
 * A206523  The pairwise sums of the terms of A004210.
 * https://oeis.org/A206523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. Generate A004210 by its greedy rule, then list all sums
 *            A004210(i)+A004210(j), i<j, in increasing order (they are all
 *            distinct). Every sum <= A004210(K) uses only the first K-1
 *            terms, so K is increased until at least n such sums are found
 * Range:     u8 n <= 50, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 80 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static long long *A004210_terms = NULL;
static idx_t A004210_tlen = 0, A004210_tcap = 0;
static uint64_t *A004210_F = NULL;
static size_t A004210_fwords = 0;

static void A004210_fset(long long x) {
    size_t w = (size_t)(x >> 6);
    if (w >= A004210_fwords) {
        size_t nw = A004210_fwords ? A004210_fwords : 1024;
        while (nw <= w) nw *= 2;
        A004210_F = (uint64_t *)realloc(A004210_F, nw * sizeof(uint64_t));
        memset(A004210_F + A004210_fwords, 0, (nw - A004210_fwords) * sizeof(uint64_t));
        A004210_fwords = nw;
    }
    A004210_F[w] |= (uint64_t)1 << (x & 63);
}

static int A004210_fget(long long x) {
    size_t w = (size_t)(x >> 6);
    if (w >= A004210_fwords) return 0;
    return (int)((A004210_F[w] >> (x & 63)) & 1);
}

static void A004210_add_term(long long v) {
    idx_t j;
    if (A004210_tlen >= A004210_tcap) {
        A004210_tcap = A004210_tcap ? A004210_tcap * 2 : 1024;
        A004210_terms = (long long *)realloc(A004210_terms, A004210_tcap * sizeof(long long));
    }
    A004210_fset(v);
    for (j = 0; j < A004210_tlen; j++) {
        A004210_fset(v - A004210_terms[j]);
        A004210_fset(v + A004210_terms[j]);
    }
    A004210_terms[A004210_tlen++] = v;
}

static void A004210_compute_next(void) {
    long long m = A004210_terms[A004210_tlen - 1] + 1;
    for (;; m++) {
        bool ok = true;
        idx_t i;
        if (A004210_fget(m)) continue;
        for (i = A004210_tlen; i-- > 0;) {
            long long t = A004210_terms[i];
            if (A004210_fget(m + t) || (m > t && A004210_fget(m - t))) { ok = false; break; }
        }
        if (ok) break;
    }
    A004210_add_term(m);
}

static long long A004210_a(idx_t n) {
    if (n < 1) return 0;
    if (A004210_tlen == 0) A004210_add_term(1);
    while (A004210_tlen < n) A004210_compute_next();
    return A004210_terms[n - 1];
}

static int A206523_cmp(const void *x, const void *y) {
    long long u = *(const long long *)x, v = *(const long long *)y;
    return (u > v) - (u < v);
}

seq_t a(idx_t n) {
    idx_t K = 4;
    if (n < 1) return 0;
    for (;;) {
        long long lim, *buf;
        idx_t i, j, cnt = 0;
        A004210_a(K);
        lim = A004210_terms[K - 1];
        buf = (long long *)malloc((size_t)((K - 1) * (K - 1) / 2 + 1) * sizeof(long long));
        for (j = 1; j < K - 1; j++)
            for (i = 0; i < j; i++) {
                long long s = A004210_terms[i] + A004210_terms[j];
                if (s <= lim) buf[cnt++] = s;
            }
        if (cnt >= n) {
            long long r;
            qsort(buf, (size_t)cnt, sizeof(long long), A206523_cmp);
            r = buf[n - 1];
            free(buf);
            return (seq_t)r;
        }
        free(buf);
        K *= 2;
    }
}
