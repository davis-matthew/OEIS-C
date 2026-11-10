/*
 * A098868  Numbers n where A098018(n)=0.
 * https://oeis.org/A098868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number m >= 1 with A098018(m) = 0, where A098018(m)
 *            = Sum_{k|m, k>=2} mu(k-1); computed by a sieve with a doubling
 *            limit
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static idx_t *A098868_terms = NULL;
static size_t A098868_cnt = 0;

static void A098868_build(size_t L)
{
    int8_t *mu = (int8_t*)calloc(L + 2, 1);
    uint8_t *comp = (uint8_t*)calloc(L + 2, 1);
    int *pr = (int*)malloc((L + 2) * sizeof(int));
    int *s = (int*)calloc(L + 2, sizeof(int));
    if (!mu || !comp || !pr || !s) abort();
    size_t pc = 0;
    mu[1] = 1;
    for (size_t i = 2; i <= L; ++i) {
        if (!comp[i]) { pr[pc++] = (int)i; mu[i] = -1; }
        for (size_t j = 0; j < pc; ++j) {
            size_t m = i * (size_t)pr[j];
            if (m > L) break;
            comp[m] = 1;
            if (i % (size_t)pr[j] == 0) { mu[m] = 0; break; }
            mu[m] = (int8_t)(-mu[i]);
        }
    }
    for (size_t d = 2; d <= L; ++d) {
        int v = mu[d - 1];
        if (v == 0) continue;
        for (size_t m = d; m <= L; m += d) s[m] += v;
    }
    size_t c = 0;
    for (size_t m = 1; m <= L; ++m) if (s[m] == 0) ++c;
    free(A098868_terms);
    A098868_terms = (idx_t*)malloc((c + 1) * sizeof(idx_t));
    if (!A098868_terms) abort();
    c = 0;
    for (size_t m = 1; m <= L; ++m) if (s[m] == 0) A098868_terms[c++] = (idx_t)m;
    A098868_cnt = c;
    free(mu); free(comp); free(pr); free(s);
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    size_t L = (size_t)(4 * n + 64);
    while (A098868_cnt < (size_t)n) {
        A098868_build(L);
        if (A098868_cnt >= (size_t)n) break;
        L *= 2;
    }
    return (seq_t)A098868_terms[n - 1];
}
