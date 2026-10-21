/*
 * A235853  Number of (n+1)X(4+1) 0..3 arrays with the upper median equal to
 *          the lower median in every 2X2 subblock.
 * https://oeis.org/A235853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A235853: number of (n+1)×5 arrays over {0,1,2,3} whose every 2×2
   sub‑block has the two middle values equal (i.e. upper median = lower median).  */

/* The algorithm builds a directed graph whose vertices are all 5‑tuples
   over {0,1,2,3}.  An edge r→c exists iff rows r and c satisfy the median
   condition for all four adjacent column pairs.  The number of admissible
   (n+1)×5 arrays is the number of length‑n directed paths in this graph,
   i.e. the sum of all entries of the vector obtained after n transitions
   starting from the all‑ones vector. */

#include <stdint.h>
#include <stdbool.h>

/* There are 4^5 = 1024 possible rows.  */
#define ROWS 1024

/* Global data (built once) */
static bool adjacency_built = false;
static uint8_t  row_digits[ROWS][5];       /* base‑4 digits of each row    */
static uint16_t children[ROWS][ROWS];     /* adjacency list              */
static unsigned int child_count[ROWS];    /* number of children for each row */

/* Forward declaration */
static void build_adjacency(void);

/* Helper: sort four integers and return true if the two middle ones are equal. */
static inline bool block_ok(int a, int b, int c, int d)
{
    int v0 = a, v1 = b, v2 = c, v3 = d;
    if (v0 > v1) { int t = v0; v0 = v1; v1 = t; }
    if (v2 > v3) { int t = v2; v2 = v3; v3 = t; }
    if (v0 > v2) { int t = v0; v0 = v2; v2 = t; }
    if (v1 > v3) { int t = v1; v1 = v3; v3 = t; }
    if (v1 > v2) { int t = v1; v1 = v2; v2 = t; }
    return v1 == v2;
}

/* Build adjacency lists between rows. */
static void build_adjacency(void)
{
    /* Precompute digits for each row. */
    for (int i = 0; i < ROWS; ++i) {
        int x = i;
        for (int j = 0; j < 5; ++j) {
            row_digits[i][j] = (uint8_t)(x & 3);
            x >>= 2;
        }
    }

    /* Compute adjacency. */
    for (int i = 0; i < ROWS; ++i) {
        child_count[i] = 0;
        for (int j = 0; j < ROWS; ++j) {
            bool ok = true;
            for (int k = 0; k < 4 && ok; ++k) {
                if (!block_ok(row_digits[i][k], row_digits[i][k + 1],
                              row_digits[j][k], row_digits[j][k + 1]))
                    ok = false;
            }
            if (ok) {
                children[i][child_count[i]++] = (uint16_t)j;
            }
        }
    }
    adjacency_built = true;
}

/* The requested sequence function */
seq_t a(idx_t n)
{
    /* Build adjacency lists once. */
    if (!adjacency_built) {
        build_adjacency();
    }

    static seq_t dp_cur[ROWS];
    static seq_t dp_next[ROWS];

    /* Initial state: one way to start with any row. */
    for (int r = 0; r < ROWS; ++r) {
        dp_cur[r] = (seq_t)1;
    }

    /* Perform n transitions to obtain n+1 rows. */
    for (idx_t step = 0; step < n; ++step) {
        /* Zero dp_next */
        for (int r = 0; r < ROWS; ++r) {
            dp_next[r] = (seq_t)0;
        }

        /* Transition */
        for (int r = 0; r < ROWS; ++r) {
            seq_t val = dp_cur[r];
            if (val == (seq_t)0) continue;
            unsigned int cnt = child_count[r];
            uint16_t *lst = children[r];
            for (unsigned int c = 0; c < cnt; ++c) {
                dp_next[lst[c]] += val;
            }
        }

        /* Swap dp_cur and dp_next */
        for (int r = 0; r < ROWS; ++r) {
            dp_cur[r] = dp_next[r];
        }
    }

    /* Sum all counts to produce the final answer. */
    seq_t result = (seq_t)0;
    for (int r = 0; r < ROWS; ++r) {
        result += dp_cur[r];
    }
    return result;
}
