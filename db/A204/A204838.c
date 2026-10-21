/*
 * A204838  Number of (n+1)X7 0..2 arrays with the permanents of all 2X2
 *          subblocks equal and nonzero.
 * https://oeis.org/A204838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 15, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A204838 – Number of (n+1)×7 0..2 arrays with all 2×2 permanents equal and nonzero.
 *
 *  The computation uses a row‑by‑row dynamic programming.  For a fixed
 *  permanent value p (1,2,3,4,5,6 or 8) two rows r and s are compatible
 *  iff for each j=0..5:  r[j]*s[j+1] + r[j+1]*s[j] == p.
 *  All arrays correspond to paths of length n in the directed graph
 *  whose vertices are the 2187 possible 7‑tuples over {0,1,2} and whose
 *  edges are the compatible pairs.  The number of arrays for a given n
 *  is the sum, over p, of the number of length‑n paths in the corresponding
 *  graph.  This DP runs in O(n·E) where E is the total number of edges
 *  (≈ 30 000 for all p combined), comfortably below the 2 s limit for n≤10 000.
 *
 *  All data structures are static and built lazily on the first call to a().
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define NUM_ROWS 2187
#define NUM_P    7
#define P_VALUES {1,2,3,4,5,6,8}

/* pre‑computed digits of each row number (base‑3 little endian) */
static unsigned char row_digits[NUM_ROWS][7];

/* adjacency data structures per p */
static size_t  *row_start[NUM_P] = {0};
static uint16_t *edge_list[NUM_P] = {0};

/* dynamic programming arrays */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;

/* flag whether initialization has been performed */
static bool initialized = false;

/* -------------------------------------------------------------------- */
/* Helper: true if rows r and s are compatible for permanent value p. */
static bool compatible(size_t r, size_t s, int p)
{
    const unsigned char *Rd = row_digits[r];
    const unsigned char *Sd = row_digits[s];
    for (int j = 0; j < 6; ++j) {
        int prod = Rd[j] * Sd[j + 1] + Rd[j + 1] * Sd[j];
        if (prod != p) return false;
    }
    return true;
}

/* -------------------------------------------------------------------- */
/* Lazy initialization of all tables. */
static void init_tables(void)
{
    /* convert row numbers to base‑3 digits */
    for (size_t r = 0; r < NUM_ROWS; ++r) {
        size_t t = r;
        for (int pos = 0; pos < 7; ++pos) {
            row_digits[r][pos] = (unsigned char)(t % 3);
            t /= 3;
        }
    }

    int p_arr[NUM_P] = P_VALUES;

    /* allocate dp arrays once */
    dp_cur   = (seq_t *)malloc(NUM_ROWS * sizeof(seq_t));
    dp_next  = (seq_t *)malloc(NUM_ROWS * sizeof(seq_t));

    for (int pi = 0; pi < NUM_P; ++pi) {
        size_t *rs = (size_t *)malloc((NUM_ROWS + 1) * sizeof(size_t));
        row_start[pi] = rs;

        /* first pass: count outgoing edges for each source row */
        size_t *cnt = (size_t *)calloc(NUM_ROWS, sizeof(size_t));
        int p = p_arr[pi];
        for (size_t r = 0; r < NUM_ROWS; ++r) {
            for (size_t s = 0; s < NUM_ROWS; ++s) {
                if (compatible(r, s, p))
                    ++cnt[r];
            }
        }

        rs[0] = 0;
        for (size_t r = 0; r < NUM_ROWS; ++r)
            rs[r + 1] = rs[r] + cnt[r];

        size_t total_edges = rs[NUM_ROWS];
        uint16_t *ed = (uint16_t *)malloc(total_edges * sizeof(uint16_t));
        edge_list[pi] = ed;

        /* second pass: fill edge lists */
        size_t *pos = (size_t *)calloc(NUM_ROWS, sizeof(size_t));
        for (size_t r = 0; r < NUM_ROWS; ++r)
            pos[r] = rs[r];

        for (size_t r = 0; r < NUM_ROWS; ++r) {
            for (size_t s = 0; s < NUM_ROWS; ++s) {
                if (compatible(r, s, p))
                    ed[pos[r]++] = (uint16_t)s;
            }
        }

        free(cnt);
        free(pos);
    }

    initialized = true;
}

/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized)
        init_tables();

    seq_t result = 0;
    int p_arr[NUM_P] = P_VALUES;

    for (int pi = 0; pi < NUM_P; ++pi) {
        /* initialise dp_cur to 1 for all rows */
        for (size_t r = 0; r < NUM_ROWS; ++r)
            dp_cur[r] = (seq_t)1;

        size_t steps = (size_t)n;
        for (size_t step = 0; step < steps; ++step) {
            /* zero dp_next */
            for (size_t r = 0; r < NUM_ROWS; ++r)
                dp_next[r] = (seq_t)0;

            const size_t *rs = row_start[pi];
            const uint16_t *edges = edge_list[pi];

            for (size_t r = 0; r < NUM_ROWS; ++r) {
                seq_t val = dp_cur[r];
                if (val == (seq_t)0)
                    continue;
                size_t start = rs[r];
                size_t end   = rs[r + 1];
                for (size_t k = start; k < end; ++k) {
                    size_t s = edges[k];
                    dp_next[s] += val;
                }
            }
            /* swap dp_cur and dp_next */
            seq_t *tmp = dp_cur;
            dp_cur = dp_next;
            dp_next = tmp;
        }

        /* accumulate answer from all final states */
        for (size_t r = 0; r < NUM_ROWS; ++r)
            result += dp_cur[r];
    }

    return result;
}
