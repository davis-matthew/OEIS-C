/*
 * A064423  Position of n-th prime in A064413 (if it begins at 2).
 * https://oeis.org/A064423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = (index of the n-th prime among the terms of A064413) - 1,
 *            i.e. its position when A064413 is read starting at 2. Computed
 *            by simulating the EKG sequence with a sieve
 * Range:     u8 n <= 32, u16 n <= 3618, u32 n <= 65536, u64 n <= 65536,
 *            u128 n <= 65536, big any n
 * Validated: standard, pass, 72 terms through n = 7981 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

static uint32_t *A064423_pos = NULL;   /* pos[k] = answer for k-th prime */
static size_t A064423_have = 0;
static size_t A064423_cap = 0;

static bool A064423_run(size_t n, uint32_t L)
{
    uint32_t *spf = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    uint32_t *ptr = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    unsigned char *used = (unsigned char *)calloc((size_t)L + 1, 1);
    uint32_t *res = (uint32_t *)calloc(n + 2, sizeof(uint32_t));
    if (!spf || !ptr || !used || !res) exit(1);
    for (uint32_t i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            for (uint32_t j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = i;
            ptr[i] = i;
        }
    }
    used[1] = 1; used[2] = 1;
    size_t cnt = 1;
    res[1] = 1;                 /* prime 2 at position 1 */
    uint32_t prev = 2;
    size_t pos = 1;             /* position of prev, starting at 2 */
    bool ok = true;
    while (cnt < n) {
        uint32_t best = UINT32_MAX;
        uint32_t x = prev;
        while (x > 1) {
            uint32_t p = spf[x];
            while (x % p == 0) x /= p;
            uint32_t c = ptr[p];
            while (c <= L && used[c]) c += p;
            ptr[p] = c;
            if (c <= L && c < best) best = c;
        }
        if (best == UINT32_MAX) { ok = false; break; }
        used[best] = 1;
        pos++;
        prev = best;
        if (spf[best] == best) res[++cnt] = (uint32_t)pos;
    }
    if (ok) {
        free(A064423_pos);
        A064423_pos = res;
        A064423_have = n;
        A064423_cap = n;
        res = NULL;
    }
    free(res);
    free(spf); free(ptr); free(used);
    return ok;
}

seq_t a(idx_t n)
{
    size_t nn = (size_t)n;
    if (nn > A064423_have) {
        size_t want = nn < 64 ? 64 : nn;
        uint64_t L = 4096;
        while (L < (uint64_t)want * 16) L <<= 1;
        while (!A064423_run(want, (uint32_t)L)) L <<= 1;
    }
    return (seq_t)A064423_pos[nn];
}
