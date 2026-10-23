/*
 * A184660  Number of (n+2)X3 0..3 arrays with each 3X3 subblock singular.
 * https://oeis.org/A184660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of (n+2)×3 arrays over {0,1,2,3} where each
 *            3×3 subblock is singular. Computed by dynamic programming over
 *            all pairs of consecutive rows
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 11, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define VALUES 4
#define ROWS   64                     /* 4^3 possible rows */
#define STATES (ROWS * ROWS)           /* pair of rows */
#define MAX_N  20000                   /* pre‑compute up to this n */

/* Pre‑computed row patterns */
static unsigned char rows[ROWS][3];

/* Transition tables for the DP */
static unsigned short edge_counts[STATES];
static unsigned short edge_offsets[STATES + 1];
static unsigned short edge_targets[STATES * ROWS]; /* max 64 edges per state */

/* DP buffers */
static seq_t dp_cur[STATES];
static seq_t dp_next[STATES];

/* Cache of computed terms */
static seq_t results[MAX_N + 1];
static unsigned int results_len = 0;

/* One‑time initialization flag */
static bool initialized = false;

/* Convert a state index into its two row indices (r1,r2) */
static inline void state_to_rows(int state, int *r1, int *r2)
{
    *r1 = state / ROWS;
    *r2 = state % ROWS;
}

/* Determinant of a 3×3 matrix formed by rows r1,r2,r3 (each row has 3 entries 0..3) */
static inline int det3(int r1, int r2, int r3)
{
    int a0 = rows[r1][0], a1 = rows[r1][1], a2 = rows[r1][2];
    int b0 = rows[r2][0], b1 = rows[r2][1], b2 = rows[r2][2];
    int c0 = rows[r3][0], c1 = rows[r3][1], c2 = rows[r3][2];
    return a0 * (b1 * c2 - b2 * c1)
         - a1 * (b0 * c2 - b2 * c0)
         + a2 * (b0 * c1 - b1 * c0);
}

/* Pre‑compute all row patterns */
static void precompute_rows(void)
{
    for (int r = 0; r < ROWS; ++r) {
        int v = r;
        rows[r][0] = v % VALUES;
        v /= VALUES;
        rows[r][1] = v % VALUES;
        v /= VALUES;
        rows[r][2] = v % VALUES;
    }
}

/* Pre‑compute valid transitions between states (determinant must be zero) */
static void precompute_transitions(void)
{
    /* Count outgoing edges per state */
    for (int s = 0; s < STATES; ++s) {
        int r1, r2;
        state_to_rows(s, &r1, &r2);
        unsigned short cnt = 0;
        for (int r3 = 0; r3 < ROWS; ++r3)
            if (det3(r1, r2, r3) == 0)
                ++cnt;
        edge_counts[s] = cnt;
    }

    /* Offsets into the global target array */
    edge_offsets[0] = 0;
    for (int s = 0; s < STATES; ++s)
        edge_offsets[s + 1] = edge_offsets[s] + edge_counts[s];

    /* Store target states */
    for (int s = 0; s < STATES; ++s) {
        int r1, r2;
        state_to_rows(s, &r1, &r2);
        unsigned short pos = edge_offsets[s];
        for (int r3 = 0; r3 < ROWS; ++r3)
            if (det3(r1, r2, r3) == 0)
                edge_targets[pos++] = (unsigned short)(r2 * ROWS + r3);
    }
}

/* One‑time initialization */
static void init(void)
{
    if (initialized)
        return;
    precompute_rows();
    precompute_transitions();

    /* Each pair of initial rows is a valid starting state */
    for (int i = 0; i < STATES; ++i) {
        dp_cur[i] = 1;
        dp_next[i] = 0;
    }
    results_len = 0;
    initialized = true;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n < 1ULL)
        return 0;
    if (n > (idx_t)MAX_N)
        return 0;   /* beyond pre‑computed range */

    init();

    while (results_len < (unsigned int)n) {
        /* Zero dp_next */
        for (int i = 0; i < STATES; ++i)
            dp_next[i] = 0;

        /* Transition step */
        for (int s = 0; s < STATES; ++s) {
            seq_t val = dp_cur[s];
            if (!val)
                continue;
            unsigned short start = edge_offsets[s];
            unsigned short end   = edge_offsets[s + 1];
            for (unsigned short idx = start; idx < end; ++idx)
                dp_next[edge_targets[idx]] += val;
        }

        /* Sum all states: this is a(n) */
        seq_t total = 0;
        for (int i = 0; i < STATES; ++i)
            total += dp_next[i];

        /* Store the result */
        ++results_len;
        results[results_len] = total;

        /* Prepare for next iteration */
        for (int i = 0; i < STATES; ++i)
            dp_cur[i] = dp_next[i];
    }

    return results[n];
}
