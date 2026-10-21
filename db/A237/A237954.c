/*
 * A237954  Number of (n+1)X(3+1) 0..3 arrays with the maximum plus the upper
 *          median plus the lower median plus the minimum of every 2X2
 *          subblock differing from its horizontal and vertical neighbors by
 *          exactly one.
 * https://oeis.org/A237954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = Number of (n+1)×4 0..3 arrays that satisfy the
 * OEIS condition.  The algorithm counts horizontal‑qualified pairs
 * of rows and then uses dynamic programming over vertical transitions.
 * It is O(n·S·E) where S is the number of horizontal‑qualified row‑pairs
 * (≤65536) and E is the average outdegree (very small). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Extract the value of the cell at column 'pos' (0..3) in a 4‑cell row
 * encoded as 8 bits (2 bits per cell). */
static inline uint8_t cell_value(uint8_t row, unsigned pos)
{
    return (row >> (2 * pos)) & 3U;
}

/* Global tables built once. */
static bool          initialized = false;
static uint8_t       pair_sum[256][256][3];   /* S for each 2×2 block */
static bool          pair_horiz_ok[256][256];
static int           pair_id[256][256];
static uint8_t      *state_prev = NULL;
static uint8_t      *state_curr = NULL;
static size_t        num_states = 0;
static size_t      *out_start = NULL;
static size_t      *out_count = NULL;
static uint16_t    *edges     = NULL;

/* Horizontal validity of a pair of rows.  All adjacent 2×2 blocks must
 * have sums differing by exactly 1 (in absolute value). */
static inline bool horiz_ok(uint8_t p, uint8_t q)
{
    for (unsigned block = 0; block < 2; ++block) {
        int diff = (int)pair_sum[p][q][block] - (int)pair_sum[p][q][block + 1];
        if (diff < 0) diff = -diff;
        if (diff != 1) return false;
    }
    return true;
}

/* Vertical validity between consecutive pair of rows (p,q) and (q,r).
 * For each block index the sums must differ by exactly 1. */
static inline bool vert_ok(uint8_t p, uint8_t q, uint8_t r)
{
    for (unsigned block = 0; block < 3; ++block) {
        int diff = (int)pair_sum[p][q][block] - (int)pair_sum[q][r][block];
        if (diff < 0) diff = -diff;
        if (diff != 1) return false;
    }
    return true;
}

/* Build all tables on first call. */
static void build_tables(void)
{
    /* 1. Compute sums per pair of rows. */
    for (unsigned int p = 0; p < 256; ++p)
        for (unsigned int q = 0; q < 256; ++q) {
            for (unsigned int block = 0; block < 3; ++block) {
                uint8_t vals[4];
                vals[0] = cell_value((uint8_t)p, block);
                vals[1] = cell_value((uint8_t)p, block + 1);
                vals[2] = cell_value((uint8_t)q, block);
                vals[3] = cell_value((uint8_t)q, block + 1);

                /* sort the four values to obtain min, lower median,
                 * upper median, max.  Since only the sum of all
                 * four is needed, we can sum after sorting. */
                /* simple insertion sort */
                for (unsigned i = 1; i < 4; ++i) {
                    uint8_t v = vals[i];
                    unsigned j = i;
                    while (j > 0 && vals[j - 1] > v) {
                        vals[j] = vals[j - 1];
                        --j;
                    }
                    vals[j] = v;
                }
                pair_sum[p][q][block] = vals[0] + vals[1] + vals[2] + vals[3];
            }
            pair_horiz_ok[p][q] = horiz_ok((uint8_t)p, (uint8_t)q);
        }

    /* 2. Enumerate horizontal‑ok states and assign ids. */
    num_states = 0;
    for (unsigned int p = 0; p < 256; ++p)
        for (unsigned int q = 0; q < 256; ++q)
            if (pair_horiz_ok[p][q]) pair_id[p][q] = (int)num_states++;

    state_prev = (uint8_t *)calloc(num_states, sizeof(uint8_t));
    state_curr = (uint8_t *)calloc(num_states, sizeof(uint8_t));
    for (unsigned int p = 0; p < 256; ++p)
        for (unsigned int q = 0; q < 256; ++q)
            if (pair_horiz_ok[p][q]) {
                unsigned int id = (unsigned int)pair_id[p][q];
                state_prev[id] = (uint8_t)p;
                state_curr[id] = (uint8_t)q;
            }

    /* 3. Compute outdegrees. */
    out_count = (size_t *)calloc(num_states, sizeof(size_t));
    for (unsigned int src = 0; src < num_states; ++src) {
        uint8_t p = state_prev[src];
        uint8_t q = state_curr[src];
        for (unsigned int r = 0; r < 256; ++r) {
            if (!pair_horiz_ok[q][r]) continue;
            if (vert_ok(p, q, (uint8_t)r))
                ++out_count[src];
        }
    }

    /* 4. Prefix sums for adjacency storage. */
    out_start = (size_t *)malloc((num_states + 1) * sizeof(size_t));
    out_start[0] = 0;
    for (size_t i = 0; i < num_states; ++i)
        out_start[i + 1] = out_start[i] + out_count[i];
    size_t total_edges = out_start[num_states];
    edges = (uint16_t *)malloc(total_edges * sizeof(uint16_t));

    /* 5. Fill edge list. */
    size_t *pos = (size_t *)calloc(num_states, sizeof(size_t));
    for (unsigned int src = 0; src < num_states; ++src) {
        uint8_t p = state_prev[src];
        uint8_t q = state_curr[src];
        for (unsigned int r = 0; r < 256; ++r) {
            if (!pair_horiz_ok[q][r]) continue;
            if (!vert_ok(p, q, (uint8_t)r)) continue;
            uint16_t dst = (uint16_t)pair_id[q][r];
            edges[out_start[src] + pos[src]] = dst;
            ++pos[src];
        }
    }
    free(pos);
    initialized = true;
}

/* ---------------- Main function ---------------- */
seq_t a(idx_t n)
{
    if (!initialized) build_tables();

    /* DP arrays: dp holds counts for current level of rows. */
    seq_t *dp      = (seq_t *)calloc(num_states, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(num_states, sizeof(seq_t));

    /* Base case: for row pair (0,1) we consider all horizontal‑ok pairs. */
    for (size_t i = 0; i < num_states; ++i)
        dp[i] = 1;

    /* For n+1 rows, we need (n) vertical transitions. */
    if (n > 0) {
        idx_t steps = n - 1;   /* vertical moves between consecutive row pairs */
        for (idx_t step = 0; step < steps; ++step) {
            for (size_t i = 0; i < num_states; ++i) dp_next[i] = 0;
            for (size_t src = 0; src < num_states; ++src) {
                seq_t val = dp[src];
                if (val == 0) continue;
                size_t begin = out_start[src];
                size_t end   = out_start[src + 1];
                for (size_t p = begin; p < end; ++p) {
                    uint16_t dst = edges[p];
                    dp_next[dst] += val;
                }
            }
            seq_t *tmp = dp;
            dp = dp_next;
            dp_next = tmp;
        }
    }

    /* Sum all final states. */
    seq_t result = 0;
    for (size_t i = 0; i < num_states; ++i)
        result += dp[i];

    free(dp);
    free(dp_next);
    return result;
}
