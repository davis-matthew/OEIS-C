/*
 * A003043  Number of Hamiltonian paths (or Gray codes) on n-cube with a
 *          marked starting node.
 * https://oeis.org/A003043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = A091299(n)/2^n, computed by a search: depth-first
 *            enumeration of Hamiltonian paths on the n-cube starting at
 *            vertex 0, taking only paths whose new dimensions appear in
 *            increasing order and multiplying the count by n!. The search
 *            prunes dead ends with bit-parallel degree checks and caches r...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   a3043_pop
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a3043_pop.h"

static int a3043_n;
static uint32_t a3043_M0[5], a3043_M1[5], a3043_nb[32];
static int a3043_N;

#define A3043_CBITS 21
static uint64_t *a3043_ckey;
static uint64_t *a3043_cval;

static uint64_t a3043_f(uint32_t U, int h, int used)
{
    int r = a3043_pop(U), d, maxd;
    uint32_t adj, A = 0, B = 0, E;
    uint64_t total = 0, key = 0;
    size_t slot = 0;
    bool cache;
    if (r == 0) return 1;
    adj = a3043_nb[h] & U;
    if (!adj) return 0;
    if (r == 1) return 1;
    for (d = 0; d < a3043_n; d++) {
        int s = 1 << d;
        uint32_t p = (((U >> s) & a3043_M0[d]) | ((U << s) & a3043_M1[d])) & U;
        B |= A & p;
        A |= p;
    }
    if (U & ~A) return 0;
    E = U & ~B & ~a3043_nb[h];
    if (E & (E - 1)) return 0;
    cache = (used == a3043_n && r >= 6 && r <= a3043_N - 4 && a3043_ckey);
    if (cache) {
        key = ((uint64_t)U | ((uint64_t)h << 32)) + 1;
        slot = (size_t)((key * 0x9E3779B97F4A7C15ULL) >> (64 - A3043_CBITS));
        if (a3043_ckey[slot] == key) return a3043_cval[slot];
    }
    maxd = used < a3043_n ? used : a3043_n - 1;
    for (d = 0; d <= maxd; d++) {
        int w = h ^ (1 << d);
        if (!((U >> w) & 1u)) continue;
        total += a3043_f(U & ~((uint32_t)1 << w), w, d == used ? used + 1 : used);
    }
    if (cache) {
        a3043_ckey[slot] = key;
        a3043_cval[slot] = total;
    }
    return total;
}

seq_t a(idx_t n)
{
    static bool done[6];
    static uint64_t memo[6];
    seq_t r, f;
    idx_t i;
    if (n < 1 || n > 5) return 0;
    if (!done[n]) {
        int N = 1 << (int)n, d, u;
        uint32_t full = (N == 32) ? 0xFFFFFFFFu : (((uint32_t)1 << N) - 1);
        a3043_n = (int)n;
        a3043_N = N;
        for (d = 0; d < a3043_n; d++) {
            a3043_M0[d] = 0;
            a3043_M1[d] = 0;
            for (u = 0; u < N; u++) {
                if (u & (1 << d)) a3043_M1[d] |= (uint32_t)1 << u;
                else a3043_M0[d] |= (uint32_t)1 << u;
            }
        }
        for (u = 0; u < N; u++) {
            a3043_nb[u] = 0;
            for (d = 0; d < a3043_n; d++) a3043_nb[u] |= (uint32_t)1 << (u ^ (1 << d));
        }
        a3043_ckey = (uint64_t *)calloc((size_t)1 << A3043_CBITS, sizeof(uint64_t));
        a3043_cval = (uint64_t *)malloc(((size_t)1 << A3043_CBITS) * sizeof(uint64_t));
        if (!a3043_cval) { free(a3043_ckey); a3043_ckey = NULL; }
        memo[n] = a3043_f(full & ~(uint32_t)1, 0, 0);
        free(a3043_ckey);
        free(a3043_cval);
        a3043_ckey = NULL;
        a3043_cval = NULL;
        done[n] = true;
    }
    f = 1;
    for (i = 2; i <= n; i++) f *= (seq_t)i;
    r = (seq_t)memo[n];
    return r * f;
}
