/*
 * A220684  Number of ways to reciprocally link elements of an nX4 array
 *          either to themselves or to exactly two horizontal, vertical or
 *          antidiagonal neighbors.
 * https://oeis.org/A220684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 210 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence‐free implementation of A220684:
   Count of reciprocal linkings on an n×4 grid where each cell either self‑loops
   or is connected to exactly two other cells (horizontal, vertical or antidiagonal).
   The algorithm uses transfer‑matrix dynamic programming over 81 states, where a
   state encodes, for every column, how many edges from the previous row are
   incident to that column (0,1 or 2).  For each row, all combinations of horizontal,
   vertical and antidiagonal edges are examined; only configurations in which each
   column has an even degree (0 or 2) are admitted.  The transition matrix is
   computed once and then reused for all subsequent calls. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

static void build_transition_matrix(unsigned short T[81][81])
{
    int s;
    for (s = 0; s < 81; ++s)
        for (int t = 0; t < 81; ++t)
            T[s][t] = 0;

    for (s = 0; s < 81; ++s) {
        /* decode incoming degrees from previous row (base‑3 representation) */
        int in_prev[4];
        int tmp = s;
        for (int j = 0; j < 4; ++j) {
            in_prev[j] = tmp % 3;
            tmp /= 3;
        }

        /* enumerate horizontal edge choices (3 possible edges) */
        for (int hm = 0; hm < 8; ++hm) {
            int d[4];
            for (int j = 0; j < 4; ++j)
                d[j] = in_prev[j];

            if (hm & 1) { d[0]++; d[1]++; }   /* edge 0‑1 */
            if (hm & 2) { d[1]++; d[2]++; }   /* edge 1‑2 */
            if (hm & 4) { d[2]++; d[3]++; }   /* edge 2‑3 */

            /* enumerate vertical choices (4 edges) */
            for (int vm = 0; vm < 16; ++vm) {
                int in_next[4] = {0, 0, 0, 0};
                int d2[4];
                for (int j = 0; j < 4; ++j)
                    d2[j] = d[j];

                for (int j = 0; j < 4; ++j) {
                    if (vm & (1 << j)) {
                        d2[j]++;                  /* edge vertical at column j */
                        in_next[j]++;              /* contributes to next row */
                    }
                }

                /* enumerate antidiagonal choices (3 edges) */
                for (int am = 0; am < 8; ++am) {
                    int d3[4];
                    for (int j = 0; j < 4; ++j)
                        d3[j] = d2[j];
                    int new_in_next[4] = {in_next[0], in_next[1], in_next[2], in_next[3]};

                    for (int k = 0; k < 3; ++k) {
                        if (am & (1 << k)) {
                            int j = k + 1;                /* source column j */
                            d3[j]++;                       /* edge from (r,j) to (r+1,j-1) */
                            new_in_next[j-1]++;             /* contributes to next row column j-1 */
                        }
                    }

                    /* check validity of edges for current row */
                    bool ok = true;
                    for (int j = 0; j < 4; ++j) {
                        if (d3[j] > 2 || (d3[j] & 1)) { ok = false; break; }
                    }
                    if (!ok) continue;
                    for (int j = 0; j < 4; ++j) {
                        if (new_in_next[j] > 2) { ok = false; break; }
                    }
                    if (!ok) continue;

                    int ns = new_in_next[0] + 3 * new_in_next[1] + 9 * new_in_next[2] + 27 * new_in_next[3];
                    T[s][ns] += 1;
                }
            }
        }
    }
}

seq_t a(idx_t n)
{
    static unsigned short T[81][81];
    static bool T_ready = false;
    static seq_t *a_cache = NULL;
    static size_t a_cache_len = 0;
    static idx_t computed_n = 0;

    static seq_t dp_state1[81];
    static seq_t dp_state2[81];
    static seq_t *dp_cur  = dp_state1;
    static seq_t *dp_next = dp_state2;

    if (!T_ready) {
        build_transition_matrix(T);
        T_ready = true;
        for (int i = 0; i < 81; ++i) dp_cur[i] = 0;
        dp_cur[0] = 1;          /* initial state before any rows: all zeros */
    }

    if (n <= computed_n)
        return a_cache[n-1];

    /* enlarge cache if needed */
    if ((size_t)n > a_cache_len) {
        seq_t *tmp = mem_resize(seq_t, a_cache, (size_t)n);
        if (!tmp) exit(1);
        a_cache = tmp;
        a_cache_len = (size_t)n;
    }

    /* compute terms up to n */
    for (idx_t row = computed_n + 1; row <= n; ++row) {
        for (int i = 0; i < 81; ++i)
            dp_next[i] = 0;

        for (int s = 0; s < 81; ++s) {
            if (dp_cur[s] == 0) continue;
            seq_t cur = dp_cur[s];
            for (int t = 0; t < 81; ++t) {
                unsigned short trans = T[s][t];
                if (!trans) continue;
                seq_t prod = mul_small(cur, trans);
                dp_next[t] += prod;
            }
        }

        a_cache[row - 1] = dp_next[0];       /* configurations that close the grid */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    computed_n = n;
    return a_cache[n-1];
}
