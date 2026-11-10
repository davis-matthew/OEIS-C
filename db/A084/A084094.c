/*
 * A084094  Positions of records in A034690.
 * https://oeis.org/A084094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th record (strict left-to-right
 *            maximum) of A034690(m), A034690(m) = sum of digit sums of
 *            divisors of m; computed by a doubling sieve
 * Range:     u8 n <= 19, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t *A084094_rec = NULL;
static size_t A084094_cnt = 0;
static uint64_t A084094_lim = 0;

static void A084094_build(uint64_t L)
{
    uint32_t *ds = (uint32_t *)malloc((L + 1) * sizeof(uint32_t));
    uint32_t *sm = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    if (!ds || !sm) { free(ds); free(sm); return; }
    ds[0] = 0;
    for (uint64_t d = 1; d <= L; ++d) ds[d] = ds[d / 10] + (uint32_t)(d % 10);
    for (uint64_t d = 1; d <= L; ++d) {
        uint32_t v = ds[d];
        for (uint64_t m = d; m <= L; m += d) sm[m] += v;
    }
    size_t c = 0;
    uint32_t best = 0;
    for (uint64_t m = 1; m <= L; ++m) {
        if (sm[m] > best) { best = sm[m]; ++c; }
    }
    uint64_t *r = (uint64_t *)malloc((c + 1) * sizeof(uint64_t));
    if (r) {
        size_t k = 0;
        best = 0;
        for (uint64_t m = 1; m <= L; ++m) {
            if (sm[m] > best) { best = sm[m]; r[k++] = m; }
        }
        free(A084094_rec);
        A084094_rec = r;
        A084094_cnt = c;
        A084094_lim = L;
    }
    free(ds);
    free(sm);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    uint64_t L = A084094_lim ? A084094_lim : 1024;
    while ((uint64_t)A084094_cnt < (uint64_t)n) {
        if (A084094_lim) L = A084094_lim * 4;
        if (L > ((uint64_t)1 << 29)) return 0;
        uint64_t before = A084094_lim;
        A084094_build(L);
        if (A084094_lim == before) return 0;
    }
    return (seq_t)A084094_rec[n - 1];
}
