/*
 * A005360  Flimsy numbers.
 * https://oeis.org/A005360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, base
 * Formula:   not stated
 * Range:     u8 n <= 158, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 79 terms through n = 990 (2026-11-01)
 * Helpers:   popcount
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/popcount.h"

static bool odd_flimsy(uint64_t m) {
    if (m <= 1) return false;
    int w = popcount(m);
    if (w <= 2) return false;
    int *orb = (int *)malloc(m * sizeof(int));
    uint64_t *rep = (uint64_t *)malloc(m * sizeof(uint64_t));
    uint64_t r, i;
    int K = 0;
    for (r = 0; r < m; r++) orb[r] = -1;
    for (r = 0; r < m; r++) {
        if (orb[r] >= 0) continue;
        uint64_t s = r;
        while (orb[s] < 0) { orb[s] = K; s = (2 * s) % m; }
        rep[K++] = r;
    }
    uint64_t ord = 0;
    { uint64_t p = 1; do { p = (2 * p) % m; ord++; } while (p != 1); }
    int *dist = (int *)malloc(K * sizeof(int));
    int *queue = (int *)malloc(K * sizeof(int));
    int k;
    for (k = 0; k < K; k++) dist[k] = -1;
    int zero = orb[0];
    int head = 0, tail = 0;
    bool found = false;
    dist[orb[1]] = 1;
    queue[tail++] = orb[1];
    while (head < tail && !found) {
        int o = queue[head++];
        int d = dist[o];
        if (d >= w - 1) break;
        uint64_t base = rep[o];
        uint64_t p = 1;
        for (i = 0; i < ord; i++) {
            uint64_t s = base + p;
            if (s >= m) s -= m;
            int t = orb[s];
            if (t == zero) { found = true; break; }
            if (dist[t] < 0) { dist[t] = d + 1; queue[tail++] = t; }
            p = (2 * p) % m;
        }
    }
    free(orb); free(rep); free(dist); free(queue);
    return found;
}

/* Search: n is flimsy iff its odd part m is. For odd m, a multiple of m with j one-bits
   is a sum of j powers of 2 that is 0 mod m, and powers of 2 mod m repeat. So a BFS over
   residues mod m (grouped into orbits under doubling), adding powers of 2 one at a time,
   finds the fewest powers of 2 summing to 0 mod m. m is flimsy iff that count < popcount(m). */

static char *status = NULL;   /* 0 unknown, 1 flimsy, 2 not */
static uint64_t status_cap = 0;
static uint64_t *terms = NULL;
static idx_t terms_len = 0, terms_cap = 0;
static uint64_t next_cand = 1;

static bool is_flimsy(uint64_t v) {
    if (v >= status_cap) {
        uint64_t nc = status_cap ? status_cap : 1024;
        while (nc <= v) nc *= 2;
        status = (char *)realloc(status, nc);
        for (uint64_t j = status_cap; j < nc; j++) status[j] = 0;
        status_cap = nc;
    }
    if (status[v]) return status[v] == 1;
    bool res;
    if ((v & 1) == 0) res = is_flimsy(v >> 1);
    else res = odd_flimsy(v);
    status[v] = res ? 1 : 2;
    return res;
}

seq_t a(idx_t n) {
    while (terms_len < n) {
        while (!is_flimsy(next_cand)) next_cand++;
        if (terms_len >= terms_cap) {
            terms_cap = terms_cap ? terms_cap * 2 : 1024;
            terms = (uint64_t *)realloc(terms, terms_cap * sizeof(uint64_t));
        }
        terms[terms_len++] = next_cand;
        next_cand++;
    }
    return (seq_t)terms[n - 1];
}
