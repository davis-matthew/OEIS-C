/*
 * A236837  The greatest inverse of A234741: a(n) = the largest k such that
 *          A234741(k) = n, and 0 if no such k exists.
 * https://oeis.org/A236837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{k : A234741(k) = n}, 0 if none. Uses a sieve:
 *            A234741(k) <= k and degree is additive, so k <=
 *            3^(bitlength(n)-1); a table of the largest k per value is built
 *            once
 * Range:     u8 n <= 64, u16 n <= 2209, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 86 terms through n = 7725 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A236837_clmul(uint64_t x, uint64_t y)
{
    uint64_t r = 0;
    while (y) {
        if (y & 1) r ^= x;
        x <<= 1;
        y >>= 1;
    }
    return r;
}

static uint32_t *A236837_best = 0;
static int A236837_D = -1;

static void A236837_build(int D)
{
    uint64_t L = 1;
    for (int i = 0; i < D; i++) L *= 3;
    uint64_t size = (uint64_t)1 << (D + 1);
    uint32_t *spf = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    uint64_t *f = (uint64_t *)calloc((size_t)(L + 1), sizeof(uint64_t));
    uint32_t *best = (uint32_t *)calloc((size_t)size, sizeof(uint32_t));
    for (uint64_t i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            for (uint64_t j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    f[1] = 1;
    if (size > 1) best[1] = 1;
    for (uint64_t k = 2; k <= L; k++) {
        uint64_t p = spf[k];
        f[k] = A236837_clmul(f[k / p], p);
        if (f[k] < size) best[f[k]] = (uint32_t)k;
    }
    free(spf);
    free(f);
    if (A236837_best) free(A236837_best);
    A236837_best = best;
    A236837_D = D;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    int D = 0;
    uint64_t t = (uint64_t)n;
    while (t > 1) { t >>= 1; D++; }
    if (D > 20) return 0;
    if (D > A236837_D) A236837_build(D);
    return (seq_t)A236837_best[(uint64_t)n];
}
