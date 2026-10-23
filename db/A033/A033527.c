/*
 * A033527  Number of matchings in graph P_{2} X P_{4} X P_{n}.
 * https://oeis.org/A033527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of matchings in the graph P2 × P4 × Pn
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a transfer–matrix approach.
   The slice graph (2 × 4) has 8 vertices; each vertex can be
   matched inside the slice, left unmatched, or matched to the next
   slice.  States are represented by an 8‑bit mask where a bit set
   means the vertex is already matched with the previous slice.
   For each pair of incoming and outgoing masks we pre‑compute the
   number of ways to complete the slice, which equals the number of
   matchings of the subgraph induced by the remaining vertices.
   The DP iterates over n slices, accumulating the number of global
   matchings.  The algorithm runs in O(n · 2^8) time (≈6.6 M
   operations for n = 10000).  It works for all supported seq_t
   integer types. */

#include <stdint.h>
#include <stdbool.h>

/* external linkage */
seq_t a(idx_t n)
{
    /* ---------- Precomputation of subgraph matchings ---------- */
    static bool initialized = false;
    static seq_t M[256];          /* M[mask] = number of matchings of subgraph mask */
    static uint8_t neigh[8];      /* adjacency bitmask for each vertex */

    if (!initialized)
    {
        /* build adjacency of the 2×4 slice graph */
        for (int v = 0; v < 8; v++) neigh[v] = 0;

        /* vertical edges between the two rows for each column */
        for (int j = 0; j < 4; j++) {
            int u = j * 2;            /* (row=0, col=j) */
            int v = u + 1;            /* (row=1, col=j) */
            neigh[u] |= 1u << v;
            neigh[v] |= 1u << u;
        }

        /* horizontal edges within each row */
        for (int row = 0; row < 2; row++) {
            for (int j = 0; j < 3; j++) {
                int u = row + j * 2;
                int v = row + (j + 1) * 2;
                neigh[u] |= 1u << v;
                neigh[v] |= 1u << u;
            }
        }

        /* dynamic programming over subsets to compute M[mask] */
        M[0] = (seq_t)1;
        for (int mask = 1; mask < 256; mask++) {
            int u = __builtin_ctz(mask);            /* least significant set bit */
            seq_t res = M[mask ^ (1 << u)];        /* vertex u left unmatched */
            uint8_t avail = neigh[u] & mask;       /* neighbors of u that are in mask */
            uint8_t sub = avail;
            while (sub) {
                int v = __builtin_ctz(sub);
                res += M[mask ^ (1 << u) ^ (1 << v)]; /* match u with v */
                sub &= sub - 1;                     /* clear lowest bit */
            }
            M[mask] = res;
        }
        initialized = true;
    }

    /* ---------- Dynamic programming over the n slices ---------- */
    const unsigned int FULL = 0xFFu;          /* all 8 vertices set */
    static seq_t dp_cur[256];
    static seq_t dp_next[256];

    /* initialise dp_cur for slice 0 */
    for (int i = 0; i < 256; i++) dp_cur[i] = (seq_t)0;
    dp_cur[0] = (seq_t)1;                     /* no vertex matched from previous slice */

    for (idx_t step = 0; step < n; step++) {
        /* reset dp_next */
        for (int i = 0; i < 256; i++) dp_next[i] = (seq_t)0;

        for (int p = 0; p < 256; p++) {
            seq_t cur_val = dp_cur[p];
            if (cur_val == (seq_t)0) continue;   /* skip zero entries */

            unsigned int comp = FULL ^ p;        /* vertices available for outgoing edges */

            unsigned int sub = comp;
            while (1) {
                unsigned int q = sub;                               /* outgoing mask */
                unsigned int rmask = FULL ^ (p | q);                /* remaining free vertices */
                seq_t val = M[rmask];                               /* ways to match inside */
                seq_t prod = cur_val * val;
                dp_next[q] += prod;

                if (sub == 0) break;
                sub = (sub - 1) & comp;   /* next submask of comp */
            }
        }

        /* move to next slice */
        for (int i = 0; i < 256; i++) dp_cur[i] = dp_next[i];
    }

    return dp_cur[0];   /* all vertices matched correctly after last slice */
}
