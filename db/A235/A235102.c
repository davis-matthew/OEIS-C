/*
 * A235102  Number of (n+1) X (3+1) 0..6 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 4,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A235102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 12, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   adjacent_rows
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A235102 – number of (n+1)×4 arrays over {0,…,6} with no equal adjacent entries
 *  and each 2×2 block having diagonal sum differing from antidiagonal sum by 4.
 *
 *  The algorithm enumerates all admissible 4‑digit rows (1512 rows),
 *  builds the undirected transition graph between rows, then counts
 *  the number of length‑(n+1) paths in that graph by dynamic programming.
 *  The graph has ~3000 edges.  Each DP step performs one edge traversal per
 *  active state, giving O(n·E) ≈ 3·10⁴ operations for n = 10 000, well within
 *  the time limit.  All arithmetic is performed on seq_t; overflows are allowed
 *  only when the result cannot fit in seq_t (as required by the exercise).
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/adjacent_rows.h"

/* ---------- global matrices built on first call ---------- */
static uint8_t         (*pattern)[4] = NULL;  /* 4‑digit rows */
static size_t          pattern_count = 0;     /* number of rows */
static int            **adjacency = NULL;    /* adjacency lists (flat arrays) */
static size_t          *adj_len    = NULL;    /* neighbour counts */

/* generate all rows with no equal horizontal neighbours */
static void
build_patterns(void)
{
    const int DIGITS = 7;
    const int WIDTH  = 4;
    const size_t MAX_ROWS = 1512; /* 7·6³ */

    pattern = (uint8_t (*)[4])malloc(MAX_ROWS * sizeof(*pattern));
    pattern_count = 0;

    for (int a = 0; a < DIGITS; ++a)
        for (int b = 0; b < DIGITS; ++b) if (b != a)
            for (int c = 0; c < DIGITS; ++c) if (c != b)
                for (int d = 0; d < DIGITS; ++d) if (d != c) {
                    pattern[pattern_count][0] = (uint8_t)a;
                    pattern[pattern_count][1] = (uint8_t)b;
                    pattern[pattern_count][2] = (uint8_t)c;
                    pattern[pattern_count][3] = (uint8_t)d;
                    ++pattern_count;
                }
}

/* build all adjacency lists (symmetric, undirected) */
static void
build_adjacency(void)
{
    adjacency = (int **)malloc(pattern_count * sizeof(int *));
    adj_len    = (size_t *)calloc(pattern_count, sizeof(size_t));

    for (size_t i = 0; i < pattern_count; ++i)
        adjacency[i] = (int *)malloc(pattern_count * sizeof(int)); /* worst‑case capacity */

    for (size_t i = 0; i < pattern_count; ++i) {
        for (size_t j = i + 1; j < pattern_count; ++j) {
            if (adjacent_rows(pattern[i], pattern[j])) {
                adjacency[i][adj_len[i]++] = (int)j;
                adjacency[j][adj_len[j]++] = (int)i;
            }
        }
    }
}

/* ---------- main sequence function ---------- */
seq_t
a(idx_t n)
{
    /* initialise static tables on first invocation */
    static bool initialized = false;
    if (!initialized) {
        build_patterns();
        build_adjacency();
        initialized = true;
    }

    /* dynamic programming arrays */
    seq_t *dp_cur  = (seq_t *)calloc(pattern_count, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(pattern_count, sizeof(seq_t));

    /* every pattern may start the first row */
    for (size_t i = 0; i < pattern_count; ++i) dp_cur[i] = (seq_t)1;

    /* propagate through n transitions → n+1 rows */
    for (idx_t step = 0; step < n; ++step) {
        /* clear next layer */
        for (size_t k = 0; k < pattern_count; ++k) dp_next[k] = (seq_t)0;

        for (size_t i = 0; i < pattern_count; ++i) {
            seq_t cur = dp_cur[i];
            if (cur == (seq_t)0) continue;
            size_t len = adj_len[i];
            int   *nbr = adjacency[i];
            for (size_t k = 0; k < len; ++k) {
                int j = nbr[k];
                dp_next[j] += cur;
            }
        }
        /* swap current and next layers */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* sum all configurations ending in any row */
    seq_t total = (seq_t)0;
    for (size_t i = 0; i < pattern_count; ++i)
        total += dp_cur[i];

    /* clean up */
    free(dp_cur);
    free(dp_next);
    return total;
}
