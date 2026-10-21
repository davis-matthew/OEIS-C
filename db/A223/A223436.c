/*
 * A223436  Generalized Petersen graph (8,2) coloring a rectangular array:
 *          number of n X 4 0..15 arrays where 0..15 label nodes of a graph
 *          with edges 0,1 0,8 8,14 8,10 1,2 1,9 9,15 9,11 2,3 2,10 10,12 3,4
 *          3,11 11,13 4,5 4,12 12,14 5,6 5,13 13,15 6,7 6,14 7,0 7,15 and
 *          every array movement to a horizontal or v...
 * https://oeis.org/A223436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A223436: count of n×4 arrays of labels 0..15 using the edges of
   the Generalized Petersen graph (8,2) as adjacency.  The value is
   computed by dynamic programming over all 432 valid rows (4‑tuples with
   horizontal edges) and counting vertical transitions allowed by the
   same graph.  The DP runs in O(n·row_count²) with a constant row_count
   and therefore O(n) time with a very small constant factor.  All
   arithmetic is performed in seq_t which may be an unsigned integer of
   8,16,32,64,128 bits or an arbitrary‑precision unsigned integer
   class.  The implementation uses only C99/C11 features and does not
   depend on new, class or template.  It is fully thread‑safe for
   sequential calls as the global state is initialized once. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* --- constants ------------------------------------------------------- */
#define ROWS_COUNT     432         /* number of valid 4‑tuples for the graph */
#define MAX_N          10000       /* maximum n that can be handled */
#define MAX_TRANS      ROWS_COUNT  /* maximum number of vertical transitions
                                       from any row (≤ROWS_COUNT) */

/* --- graph adjacency (undirected) ----------------------------------- */
static const unsigned char edge_list[][2] = {
    {0,1},{0,8},{8,14},{8,10},{1,2},{1,9},{9,15},{9,11},
    {2,3},{2,10},{10,12},{3,4},{3,11},
    {11,13},{4,5},{4,12},{12,14},
    {5,6},{5,13},{13,15},{6,7},{6,14},
    {7,0},{7,15}
};

/* --- static tables --------------------------------------------------- */
static bool neighbor[16][16] = {0};

static uint16_t rows[ROWS_COUNT];
static uint8_t  row_lbl[ROWS_COUNT][4];

static uint16_t trans[ROWS_COUNT][MAX_TRANS];
static uint16_t trans_cnt[ROWS_COUNT];

static seq_t dp[ROWS_COUNT];
static seq_t dp_next[ROWS_COUNT];
static seq_t *cur = dp;
static seq_t *next = dp_next;

static seq_t a_values[MAX_N];
static size_t  a_len   = 0;
static idx_t   current_n = 0;
static bool    initialized = false;

/* --------------------------------------------------------------------- */
static void init(void)
{
    /* build adjacency matrix */
    for (size_t i = 0; i < sizeof(edge_list)/sizeof(edge_list[0]); ++i) {
        unsigned u = edge_list[i][0];
        unsigned v = edge_list[i][1];
        neighbor[u][v] = neighbor[v][u] = true;
    }

    /* enumerate all valid rows (horizontal edge constraints) */
    size_t rcnt = 0;
    for (uint8_t a = 0; a < 16; ++a) {
        for (uint8_t b = 0; b < 16; ++b) {
            if (!neighbor[a][b]) continue;
            for (uint8_t c = 0; c < 16; ++c) {
                if (!neighbor[b][c]) continue;
                for (uint8_t d = 0; d < 16; ++d) {
                    if (!neighbor[c][d]) continue;
                    rows[rcnt] = ((uint16_t)a << 12) | ((uint16_t)b << 8) |
                                 ((uint16_t)c << 4)  | ((uint16_t)d);
                    row_lbl[rcnt][0] = a;
                    row_lbl[rcnt][1] = b;
                    row_lbl[rcnt][2] = c;
                    row_lbl[rcnt][3] = d;
                    ++rcnt;
                }
            }
        }
    }

    /* pre‑compute vertical transitions */
    for (size_t i = 0; i < rcnt; ++i) {
        trans_cnt[i] = 0;
        for (size_t j = 0; j < rcnt; ++j) {
            bool ok = true;
            for (int k = 0; k < 4; ++k) {
                if (!neighbor[row_lbl[i][k]][row_lbl[j][k]]) { ok = false; break; }
            }
            if (ok) trans[i][trans_cnt[i]++] = (uint16_t)j;
        }
    }

    /* zero DP arrays */
    for (size_t i = 0; i < rcnt; ++i) { dp[i] = 0; dp_next[i] = 0; }
    /* zero a_values array */
    for (size_t i = 0; i < MAX_N; ++i) a_values[i] = 0;

    initialized = true;
}

/* --------------------------------------------------------------------- */
/* Public sequence function                                              */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) init();

    /* only n >= 1 is required; behaviour for n < 1 is undefined */
    if (n == 0) return 0;

    if (n > MAX_N) {
        /* The implementation only guarantees correct values up to MAX_N.
           For larger n a fallback may be added. */
        return 0;
    }

    /* extend memoised values up to requested n */
    while (current_n < n) {
        if (current_n == 0) {
            /* a(1): start with one way for each valid row */
            for (size_t i = 0; i < ROWS_COUNT; ++i) cur[i] = 1;
        } else {
            /* compute dp_next from cur */
            for (size_t i = 0; i < ROWS_COUNT; ++i) next[i] = 0;
            for (size_t i = 0; i < ROWS_COUNT; ++i) {
                if (cur[i] == 0) continue;
                uint16_t tcnt = trans_cnt[i];
                for (uint16_t t = 0; t < tcnt; ++t) {
                    uint16_t j = trans[i][t];
                    next[j] += cur[i];
                }
            }
            /* swap current and next */
            seq_t *tmp = cur; cur = next; next = tmp;
        }

        /* sum current to obtain a(current_n + 1) */
        seq_t total = 0;
        for (size_t i = 0; i < ROWS_COUNT; ++i) total += cur[i];

        a_values[current_n] = total;
        ++current_n;
    }

    return a_values[n - 1];
}
