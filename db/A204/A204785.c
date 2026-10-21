/*
 * A204785  Number of (n+1)X3 0..7 arrays with the permanents of all 2X2
 *          subblocks equal and nonzero.
 * https://oeis.org/A204785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 17, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A204785: Count of (n+1)×3 arrays (entries 0..7) whose
 * permanents of all 2×2 subblocks are equal and non‑zero.
 * Computed by dynamic programming over rows.
 * For each pair of consecutive rows r and s we require
 *      r0*s1 + r1*s0  =  r1*s2 + r2*s1 = C > 0 ,
 * where C is the common permanent.
 */

/* required includes for the generic types */
#include <stdint.h>
#include <stddef.h>

/* local edge representation */
typedef struct {
    uint16_t from;
    uint16_t to;
    uint8_t   C;          /* permanent value, 1..98 (0 unused) */
} edge_t;

/* maximum possible number of pairs (512×512) = 262144 */
#define MAX_EDGES 262144

/* static storage for edge list */
static edge_t edges[MAX_EDGES];
static size_t edge_count = 0;

/* helper to decode a row index (0..511) into its three entries */
static void decode_row(uint16_t row, int *a, int *b, int *c) {
    *a = (row >> 6) & 7;     /* high 3 bits */
    *b = (row >> 3) & 7;     /* middle 3 bits */
    *c = row & 7;            /* low 3 bits */
}

/* Build the list of valid edges once (called on first invocation) */
static void build_edges(void) {
    if (edge_count != 0) return;  /* already built */

    for (uint16_t p = 0; p < 512; ++p) {
        int a, b, c;
        decode_row(p, &a, &b, &c);
        for (uint16_t q = 0; q < 512; ++q) {
            int d, e, f;
            decode_row(q, &d, &e, &f);
            int perm1 = a * e + b * d;
            int perm2 = b * f + c * e;
            if (perm1 == perm2 && perm1 > 0) {
                edges[edge_count].from = p;
                edges[edge_count].to   = q;
                edges[edge_count].C    = (uint8_t)perm1;
                ++edge_count;
            }
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 1) return 0;            /* undefined below offset, return 0 */

    build_edges();   /* ensure edges are available */

    /* DP arrays: dp_cur and dp_next indexed by [row][C] (C=1..98) */
    static seq_t dp0[512][99];
    static seq_t dp1[512][99];
    seq_t (*dp_cur)[99] = dp0;
    seq_t (*dp_next)[99] = dp1;

    /* initialize current dp to zeros */
    for (size_t r = 0; r < 512; ++r)
        for (size_t c = 1; c < 99; ++c)
            dp_cur[r][c] = 0;

    /* iterate over the number of edges (i.e., number of adjacent 2×2
     * subblocks), which equals n. */
    for (idx_t step = 1; step <= n; ++step) {
        /* zero the next array */
        for (size_t r = 0; r < 512; ++r)
            for (size_t c = 1; c < 99; ++c)
                dp_next[r][c] = 0;

        if (step == 1) {
            /* base step: every valid edge forms a new array (length 2) */
            for (size_t i = 0; i < edge_count; ++i) {
                uint16_t to   = edges[i].to;
                uint8_t   C   = edges[i].C;
                dp_next[to][C] = dp_next[to][C] + (seq_t)1;
            }
        } else {
            /* subsequent steps: extend existing sequences by one more row */
            for (size_t i = 0; i < edge_count; ++i) {
                uint16_t  from = edges[i].from;
                uint16_t  to   = edges[i].to;
                uint8_t   C    = edges[i].C;
                if (dp_cur[from][C] != 0) {
                    dp_next[to][C] = dp_next[to][C] + dp_cur[from][C];
                }
            }
        }

        /* swap pointers for next iteration */
        seq_t (*tmp)[99] = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* sum all counts to obtain a(n) */
    seq_t total = 0;
    for (size_t r = 0; r < 512; ++r)
        for (size_t c = 1; c < 99; ++c)
            total = total + dp_cur[r][c];

    return total;
}
