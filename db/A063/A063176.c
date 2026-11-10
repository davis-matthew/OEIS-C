/*
 * A063176  Composites for which the row of the prime-composite array
 *          (A063173) includes the leftmost element of both a zero-only
 *          antidiagonal and a zero-only diagonal(A067681).
 * https://oeis.org/A063176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. Let c(m) be the m-th composite and p(j) the j-th prime.
 *            a(n) is the n-th composite c(m) for which c(m) is odd
 *            (T(m,1)=0), p(1+k) does not divide c(m-k) for k=1..m-1 (zero
 *            antidiagonal), and p(1+k) does not divide c(m+k) for all k>=1
 *            (zero diagonal). Implemented with a sieve that doubles i...
 * Range:     u8 n <= 5, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *A063176_terms = NULL;
static size_t A063176_cnt = 0;

static void A063176_build(uint32_t N) {
    unsigned char *flag = (unsigned char *)calloc((size_t)N + 1, 1);
    uint32_t *P = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)N / 2 + 16));
    uint32_t *C = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)N + 16));
    size_t np = 0, nc = 0;
    uint32_t i;
    uint64_t j;
    for (i = 2; i <= N; i++) {
        if (!flag[i]) {
            P[np++] = i;
            for (j = (uint64_t)i * i; j <= N; j += i) flag[j] = 1;
        } else if (i >= 4) {
            C[nc++] = i;
        }
    }
    free(A063176_terms);
    A063176_terms = (uint32_t *)malloc(sizeof(uint32_t) * (nc + 1));
    A063176_cnt = 0;
    for (size_t m = 0; m < nc; m++) {
        if (C[m] > N / 2) break;
        if ((C[m] & 1u) == 0) continue;
        int ok = 1;
        /* antidiagonal */
        for (size_t k = 1; k <= m; k++) {
            if (k >= np) break;
            if (P[k] > C[m - k]) break;
            if (C[m - k] % P[k] == 0) { ok = 0; break; }
        }
        if (!ok) continue;
        /* diagonal */
        int range_fail = 0;
        for (size_t k = 1;; k++) {
            if (m + k >= nc || k >= np) { range_fail = 1; break; }
            if (P[k] > C[m + k]) break;
            if (C[m + k] % P[k] == 0) { ok = 0; break; }
        }
        if (range_fail) break;
        if (ok) A063176_terms[A063176_cnt++] = C[m];
    }
    free(flag);
    free(P);
    free(C);
}

seq_t a(idx_t n) {
    static uint32_t N = 0;
    if (N == 0) { N = 1u << 16; A063176_build(N); }
    while ((idx_t)A063176_cnt < n) {
        N *= 2;
        A063176_build(N);
    }
    return (seq_t)A063176_terms[n - 1];
}
