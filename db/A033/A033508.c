/*
 * A033508  Number of matchings in graph P_{5} X P_{n}.
 * https://oeis.org/A033508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) counts the matchings in the Cartesian product P5 × Pn.
   The DP processes the n columns of the 5×n grid column by column.
   Each state is a 5‑bit mask indicating which vertices in the current
   column are already matched to the previous column (incoming horizontal
   edge).  Transitions are precomputed in a 32×32 table t[s][s′]
   (s and s′ must be disjoint).  The number of ways to match the
   remaining unmatched vertices vertically depends only on the set of
   free vertices, which is encoded as a bit mask.  The DP runs in O(n)
   time and uses only 32 states. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Precompute the transition table t[s][s'] once. */
static void build_transition_table(int t[32][32]) {
    /* m[mask] = number of matchings in the subgraph induced by vertices
       highlighted in mask (5 bits). */
    int m[32] = {0};
    for (int mask = 0; mask < 32; ++mask) {
        /* enumerate all subsets of the 4 possible vertical edges: (0-1),(1-2),(2-3),(3-4) */
        int cnt = 0;
        for (int edges = 0; edges < 16; ++edges) {
            bool ok = true;
            int used = 0;
            for (int e = 0; e < 4; ++e) {
                if (edges & (1 << e)) {
                    int u = e;       /* left vertex of edge e */
                    int v = e + 1;   /* right vertex */
                    /* Both endpoints must belong to mask */
                    if (!((mask >> u) & 1) || !((mask >> v) & 1)) {
                        ok = false; break;
                    }
                    /* Vertex cannot be used twice */
                    if ((used >> u) & 1 || (used >> v) & 1) {
                        ok = false; break;
                    }
                    used |= (1 << u) | (1 << v);
                }
            }
            if (ok) ++cnt;
        }
        m[mask] = cnt;
    }

    for (int s = 0; s < 32; ++s) {
        for (int sp = 0; sp < 32; ++sp) {
            if (s & sp) {          /* cannot share vertices */
                t[s][sp] = 0;
                continue;
            }
            int free_mask = (~(s | sp)) & 31;   /* vertices free to be matched vertically */
            t[s][sp] = m[free_mask];
        }
    }
}

/* Column‑by‑column DP */
seq_t a(idx_t n) {
    /* Transition table, built once */
    static int t[32][32];
    static bool initialized = false;
    if (!initialized) {
        build_transition_table(t);
        initialized = true;
    }

    /* Allocate two DP arrays */
    static seq_t *cur = NULL;
    static seq_t *next = NULL;
    static size_t dp_cap = 0;
    if (dp_cap < 32) {
        mem_free(cur); mem_free(next);
        cur = mem_new(seq_t, 32);
        next = mem_new(seq_t, 32);
        dp_cap = 32;
    }

    /* Initialize DP: before any columns, no incoming horizontal edges */
    for (size_t i = 0; i < 32; ++i) cur[i] = 0;
    cur[0] = 1;

    /* Process n columns */
    for (idx_t col = 0; col < n; ++col) {
        for (size_t i = 0; i < 32; ++i) next[i] = 0;
        for (int s = 0; s < 32; ++s) {
            if (cur[s] == 0) continue;
            seq_t cur_val = cur[s];
            for (int sp = 0; sp < 32; ++sp) {
                if (s & sp) continue;               /* disjoint constraint */
                int ways = t[s][sp];
                if (ways == 0) continue;
                next[sp] += cur_val * (seq_t)ways;
            }
        }
        /* swap cur and next */
        seq_t *tmp = cur; cur = next; next = tmp;
    }

    /* After all columns, no outgoing horizontal edges are allowed */
    return cur[0];
}
