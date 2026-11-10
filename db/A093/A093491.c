/*
 * A093491  Index of the first occurrence of n in A093489.
 * https://oeis.org/A093491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m>=1 with A093489(m) = n; computed by search with a
 *            signature sieve
 * Range:     u8 n <= 57, u16 n <= 265, u32 n <= 265, u64 n <= 265,
 *            u128 n <= 265, big any n
 * Validated: standard, pass, 76 terms through n = 245 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static uint32_t *A093491_vals = NULL;
static uint64_t A093491_M = 0;

static void A093491_build(uint64_t M)
{
    uint64_t L = 2 * M + 1;
    uint32_t *spf = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    uint32_t *id = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    uint32_t *vals = (uint32_t *)calloc((size_t)(M + 1), sizeof(uint32_t));
    static const uint32_t sp[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    uint64_t i, j;
    if (!spf || !id || !vals) { free(spf); free(id); free(vals); return; }
    for (i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    id[1] = 1;
    for (i = 2; i <= L; i++) {
        uint32_t ex[16];
        int len = 0, a, b;
        uint64_t x = i;
        uint64_t prod = 1;
        while (x > 1) {
            uint32_t p = spf[x];
            uint32_t c = 0;
            while (x % p == 0) { x /= p; c++; }
            ex[len++] = c;
        }
        for (a = 1; a < len; a++) {
            uint32_t v = ex[a];
            b = a;
            while (b > 0 && ex[b - 1] < v) { ex[b] = ex[b - 1]; b--; }
            ex[b] = v;
        }
        for (a = 0; a < len; a++)
            for (uint32_t c = 0; c < ex[a]; c++) prod *= sp[a];
        id[i] = (uint32_t)prod;
    }
    for (i = 1; i <= M; i++) {
        uint32_t cnt = 0;
        for (j = 1; j < i; j++)
            if (id[i - j] == id[i + j]) cnt++;
        vals[i] = cnt;
    }
    free(spf);
    free(id);
    free(A093491_vals);
    A093491_vals = vals;
    A093491_M = M;
}

seq_t a(idx_t n)
{
    if (n > 0xFFFFFFFFULL) return 0;
    uint32_t target = (uint32_t)n;
    uint64_t start = 1;
    if (A093491_M == 0) A093491_build(1024);
    for (;;) {
        uint64_t m;
        if (A093491_M == 0) return 0;
        for (m = start; m <= A093491_M; m++)
            if (A093491_vals[m] == target) return (seq_t)m;
        start = A093491_M + 1;
        A093491_build(A093491_M * 2);
    }
}
