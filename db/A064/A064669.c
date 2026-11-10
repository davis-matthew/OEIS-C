/*
 * A064669  Regard A064413 as giving a permutation of the positive integers;
 *          sequence gives (presumed) smallest term in each cycle of this
 *          permutation.
 * https://oeis.org/A064669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th smallest m such that m is the least element of its
 *            cycle in the permutation k -> A064413(k). Computed by sieve and
 *            search: generate the EKG sequence, then follow forward and
 *            backward orbits of each m and keep those that never drop below m
 * Range:     u8 n <= 15, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static uint32_t *g_terms = NULL;
static size_t g_nt = 0;
static uint32_t g_N = 0;

static void A064669_build(uint32_t N)
{
    uint32_t V = 2 * N;
    uint32_t *seq = (uint32_t *)calloc((size_t)N + 2, sizeof(uint32_t));
    uint32_t *pos = (uint32_t *)calloc((size_t)V + 2, sizeof(uint32_t));
    uint32_t *spf = (uint32_t *)calloc((size_t)V + 2, sizeof(uint32_t));
    uint32_t *ptr = (uint32_t *)calloc((size_t)V + 2, sizeof(uint32_t));
    unsigned char *used = (unsigned char *)calloc((size_t)V + 2, 1);
    uint32_t i, j, n, last = 2;
    size_t cap = 64;
    uint32_t *terms;
    uint32_t lim, m;

    if (!seq || !pos || !spf || !ptr || !used) exit(1);
    for (i = 2; i <= V; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= V; j += i) {
                if (spf[j] == 0) spf[j] = i;
                if (j > V - i) break;
            }
        }
    }
    seq[1] = 1; seq[2] = 2;
    used[1] = used[2] = 1;
    pos[1] = 1; pos[2] = 2;
    for (n = 3; n <= N; n++) {
        uint32_t x = seq[n - 1];
        uint32_t best = 0xffffffffu;
        while (x > 1) {
            uint32_t p = spf[x];
            uint32_t c;
            while (x % p == 0) x /= p;
            c = ptr[p] ? ptr[p] : p;
            while (c <= V && used[c]) c += p;
            ptr[p] = c;
            if (c < best) best = c;
        }
        if (best > V) break;
        used[best] = 1;
        seq[n] = best;
        pos[best] = n;
        last = n;
    }

    lim = last / 64;
    terms = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!terms) exit(1);
    g_nt = 0;
    for (m = 1; m <= lim; m++) {
        bool ok = true;
        uint32_t x = m;
        for (;;) {
            if (x > last) break;
            x = seq[x];
            if (x == m) break;
            if (x < m) { ok = false; break; }
        }
        if (!ok) continue;
        if (x != m) {
            uint32_t safe = last / 2 - 10;
            x = m;
            for (;;) {
                if (x > safe) break;
                x = pos[x];
                if (x == 0) break;
                if (x == m) break;
                if (x < m) { ok = false; break; }
            }
        }
        if (!ok) continue;
        if (g_nt == cap) {
            cap *= 2;
            terms = (uint32_t *)realloc(terms, cap * sizeof(uint32_t));
            if (!terms) exit(1);
        }
        terms[g_nt++] = m;
    }
    free(g_terms);
    g_terms = terms;
    g_N = N;
    free(seq); free(pos); free(spf); free(ptr); free(used);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (g_N == 0) A064669_build(1u << 21);
    while ((idx_t)g_nt < n && g_N < (1u << 28)) {
        A064669_build(g_N * 2);
    }
    if ((idx_t)g_nt < n) return (seq_t)0;
    return (seq_t)g_terms[(size_t)(n - 1)];
}
