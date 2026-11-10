/*
 * A064668  Regard A064413 as giving a permutation of the positive integers;
 *          sequence gives second infinite cycle, reading backwards beginning
 *          at its smallest term, 73.
 * https://oeis.org/A064668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 73; a(k+1) = position of a(k) in the EKG sequence A064413
 *            (inverse permutation, i.e. the cycle read backwards). Computed
 *            by generating A064413 and inverting it
 * Range:     u8 n <= 8, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

static uint32_t *A064668_pos = NULL;
static uint64_t A064668_N = 0;

static void A064668_build(uint64_t N)
{
    uint64_t L = 8 * N + 16;
    uint32_t *spf = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    unsigned char *used = (unsigned char *)calloc((size_t)L + 1, 1);
    uint32_t *ptr = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    uint32_t *pos = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    uint64_t i, j;
    if (!spf || !used || !ptr || !pos) exit(1);
    for (i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    used[1] = 1; used[2] = 1;
    pos[1] = 1; pos[2] = 2;
    uint64_t prev = 2;
    for (i = 3; i <= N; i++) {
        uint64_t x = prev, best = 0;
        while (x > 1) {
            uint32_t p = spf[x];
            while (x % p == 0) x /= p;
            if (ptr[p] == 0) ptr[p] = p;
            while (ptr[p] <= L && used[ptr[p]]) ptr[p] += p;
            if (ptr[p] <= L && (best == 0 || ptr[p] < best)) best = ptr[p];
        }
        if (best == 0) exit(1);
        used[best] = 1;
        pos[best] = (uint32_t)i;
        prev = best;
    }
    free(spf); free(used); free(ptr);
    free(A064668_pos);
    A064668_pos = pos;
    A064668_N = N;
}

seq_t a(idx_t n)
{
    if (A064668_N == 0) A064668_build(4000);
    for (;;) {
        uint64_t x = 73, k;
        bool ok = true;
        for (k = 1; k < (uint64_t)n; k++) {
            if (x > A064668_N) { ok = false; break; }
            uint32_t p = A064668_pos[x];
            if (p == 0) { ok = false; break; }
            x = p;
        }
        if (ok) return (seq_t)x;
        A064668_build(A064668_N * 2);
    }
}
