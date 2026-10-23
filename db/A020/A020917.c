/*
 * A020917  Maximum number of K4's (complete 4 graphs) a graph can contain if
 *          it contains at most n distinct K3's (triangles).
 * https://oeis.org/A020917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) is the maximum number of K4 subgraphs in a graph with at
 *            most n K3 subgraphs. The algorithm computes all component types
 *            consisting of a clique and additional vertices attached to
 *            subsets of that clique, then performs an unbounded knapsack over
 *            the component types and finally takes a prefix...
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef MAX_N
#define MAX_N 20000       /* enough for n up to about 10000 */
#endif

/* precomputed sequence values */
static seq_t seq_static[MAX_N + 1];

/* one‑time initialization of the sequence */
static void init_seq(void)
{
    static bool done = false;
    if (done) return;

    /* DP containers, using signed 64‑bit integers to distinguish unreachable states */
    static int64_t comp_best[MAX_N + 1];
    static int64_t best_exact[MAX_N + 1];
    static int64_t best_extra[MAX_N + 1];

    const int64_t NEG = -1;
    for (idx_t i = 0; i <= MAX_N; ++i) comp_best[i] = best_exact[i] = NEG;

    /* enumerate component types: base clique of size m and extra vertices */
    const idx_t Mmax = 50;                     /* C(50,3)=19600 > MAX_N */
    for (idx_t m = 3; m <= Mmax; ++m) {
        unsigned long long baseW3 = (unsigned long long)m * (m - 1) * (m - 2) / 6;
        if (baseW3 > MAX_N) break;
        unsigned long long baseW4 = (unsigned long long)m * (m - 1) * (m - 2) * (m - 3) / 24;
        idx_t maxCap = MAX_N - (idx_t)baseW3;

        /* reset best_extra */
        for (idx_t i = 0; i <= maxCap; ++i) best_extra[i] = NEG;
        best_extra[0] = 0;

        for (idx_t r = 3; r <= m; ++r) {
            unsigned long long w = (unsigned long long)r * (r - 1) / 2;
            unsigned long long v = (unsigned long long)r * (r - 1) * (r - 2) / 6;
            for (idx_t c = (idx_t)w; c <= maxCap; ++c) {
                if (best_extra[c - w] != NEG) {
                    int64_t cand = best_extra[c - w] + (int64_t)v;
                    if (cand > best_extra[c]) best_extra[c] = cand;
                }
            }
        }

        for (idx_t cap = 0; cap <= maxCap; ++cap) {
            if (best_extra[cap] == NEG) continue;
            idx_t t = baseW3 + cap;
            int64_t val = (int64_t)baseW4 + best_extra[cap];
            if (val > comp_best[t]) comp_best[t] = val;
        }
    }

    /* unbounded knapsack over component types to get best value for each exact triangle count */
    best_exact[0] = 0;
    for (idx_t w = 1; w <= MAX_N; ++w) {
        int64_t val = comp_best[w];
        if (val <= 0) continue;          /* skip components with non‑positive value */
        for (idx_t t = w; t <= MAX_N; ++t) {
            if (best_exact[t - w] == NEG) continue;
            int64_t cand = best_exact[t - w] + val;
            if (cand > best_exact[t]) best_exact[t] = cand;
        }
    }

    /* prefix maximum to account for “at most n” triangles */
    int64_t cur = 0;
    for (idx_t i = 0; i <= MAX_N; ++i) {
        if (best_exact[i] != NEG && best_exact[i] > cur) cur = best_exact[i];
        seq_static[i] = (seq_t)cur;
    }

    done = true;
}

/* required function */
seq_t a(idx_t n)
{
    if (n > MAX_N) return 0;
    init_seq();
    return seq_static[n];
}
