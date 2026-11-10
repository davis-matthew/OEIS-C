/*
 * A050687  Numbers k such that A000422(k) is squarefree.
 * https://oeis.org/A050687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, hard
 * Formula:   a(n) = n-th k with A000422(k) squarefree, A000422(k) =
 *            k*10^len(A000422(k-1)) + A000422(k-1), A000422(1)=1; computed by
 *            a sieve: for each prime p <= 10^6, track A000422(k) mod p^2 and
 *            exclude k where it is 0 (the remaining k are taken as
 *            squarefree)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A050687_terms = 0;
static size_t A050687_cnt = 0;
static size_t A050687_K = 0;

static void A050687_build(size_t K) {
    const uint32_t LIM = 1000000;
    char *comp = (char *)calloc(LIM + 1, 1);
    char *mark = (char *)calloc(K + 2, 1);
    if (!comp || !mark) { free(comp); free(mark); return; }
    for (uint32_t i = 2; i <= LIM; i++) {
        if (comp[i]) continue;
        for (uint64_t j = (uint64_t)i * i; j <= LIM; j += i) comp[j] = 1;
        uint64_t m = (uint64_t)i * i;
        uint64_t res = 1 % m, pw = 10 % m;
        for (size_t k = 2; k <= K; k++) {
            res = ((uint64_t)k * pw + res) % m;
            if (res == 0) mark[k] = 1;
            size_t t = k;
            while (t > 0) { pw = (pw * 10) % m; t /= 10; }
        }
    }
    free(A050687_terms);
    A050687_terms = (uint64_t *)malloc((K + 1) * sizeof(uint64_t));
    A050687_cnt = 0;
    for (size_t k = 1; k <= K; k++)
        if (!mark[k]) A050687_terms[A050687_cnt++] = k;
    A050687_K = K;
    free(comp);
    free(mark);
}

seq_t a(idx_t n) {
    if (A050687_K == 0) A050687_build(256);
    while ((uint64_t)n > A050687_cnt && A050687_K < 200000)
        A050687_build(A050687_K * 2);
    if (n < 1 || (uint64_t)n > A050687_cnt) return 0;
    return (seq_t)A050687_terms[n - 1];
}
