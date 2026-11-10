/*
 * A064794  Minimal representatives for the finite cycles in the permutation
 *          defined by A064413.
 * https://oeis.org/A064794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th smallest m such that m lies on a finite cycle of the
 *            permutation k -> A064413(k) (EKG sequence); computed by
 *            generating the EKG sequence up to N terms and following orbits
 *            (search, with N doubled until N >= 8*a(n))
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

static uint32_t *g_mins = NULL;
static size_t g_cnt = 0;
static uint64_t g_N = 0;

static void ekg_build_cycles(uint64_t N)
{
    uint64_t V = 2 * N + 1000;
    uint32_t *seq = (uint32_t *)malloc((N + 2) * sizeof(uint32_t));
    uint8_t *used = (uint8_t *)calloc(V + 2, 1);
    uint32_t *ptr = (uint32_t *)malloc((V + 2) * sizeof(uint32_t));
    uint32_t *spf = (uint32_t *)calloc(V + 2, sizeof(uint32_t));
    uint8_t *vis = (uint8_t *)calloc(N + 2, 1);
    uint32_t *primes = (uint32_t *)malloc((V / 2 + 10) * sizeof(uint32_t));
    size_t pc = 0;
    uint64_t i, j;
    if (!seq || !used || !ptr || !spf || !vis || !primes) exit(1);

    for (i = 2; i <= V; i++) {
        if (spf[i] == 0) { spf[i] = (uint32_t)i; primes[pc++] = (uint32_t)i; }
        for (j = 0; j < pc; j++) {
            uint64_t p = primes[j];
            if (p > spf[i] || p * i > V) break;
            spf[p * i] = (uint32_t)p;
        }
    }
    for (i = 0; i <= V; i++) ptr[i] = (uint32_t)i;

    for (i = 0; i <= N + 1; i++) seq[i] = 0;
    seq[1] = 1;
    if (N >= 2) seq[2] = 2;
    used[1] = 1; used[2] = 1;
    for (i = 3; i <= N; i++) {
        uint32_t x = seq[i - 1];
        uint64_t best = (uint64_t)V + 1;
        while (x > 1) {
            uint32_t p = spf[x];
            uint64_t c = ptr[p];
            while (c <= V && used[c]) c += p;
            if (c > V) c = V + 1;
            ptr[p] = (uint32_t)(c > V ? V + 1 : c);
            if (c < best) best = c;
            while (x % p == 0) x /= p;
        }
        if (best > V) break;
        used[best] = 1;
        seq[i] = (uint32_t)best;
    }

    free(g_mins);
    g_mins = (uint32_t *)malloc(1024 * sizeof(uint32_t));
    size_t cap = 1024;
    g_cnt = 0;
    for (i = 1; i <= N; i++) {
        if (vis[i]) continue;
        uint64_t x = i;
        for (;;) {
            vis[x] = 1;
            uint64_t y = seq[x];
            if (y == 0) break;
            if (y == i) {
                if (g_cnt == cap) {
                    cap *= 2;
                    g_mins = (uint32_t *)realloc(g_mins, cap * sizeof(uint32_t));
                    if (!g_mins) exit(1);
                }
                g_mins[g_cnt++] = (uint32_t)i;
                break;
            }
            if (y > N || vis[y]) break;
            x = y;
        }
    }
    g_N = N;
    free(seq); free(used); free(ptr); free(spf); free(vis); free(primes);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (g_N == 0) ekg_build_cycles(1u << 14);
    while (!((uint64_t)n <= g_cnt && g_N >= 8 * (uint64_t)g_mins[n - 1])) {
        if (g_N >= ((uint64_t)1 << 27)) {
            if ((uint64_t)n <= g_cnt) break;
            return (seq_t)0;
        }
        ekg_build_cycles(g_N * 2);
    }
    return (seq_t)g_mins[n - 1];
}
