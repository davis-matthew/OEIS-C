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
 * Helpers:   build_transition_matrix, mul_small
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
#include "../utils/mul_small.h"
#include "../utils/build_transition_matrix.h"

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
        seq_t *tmp = (seq_t *)realloc(a_cache, (size_t)n * sizeof(seq_t));
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
