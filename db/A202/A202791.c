/*
 * A202791  Number of nX4 binary arrays with every one adjacent to another one
 *          horizontally, diagonally, antidiagonally or vertically.
 * https://oeis.org/A202791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes a(n), n ≥ 1, where a(n) is the number of n×4 binary arrays
 *  in which every 1 has at least one adjacent 1 horizontally, vertically,
 *  diagonally or antidiagonally.
 *
 *  The values are obtained by dynamic programming on the rows.  For each
 *  row we keep a 4‑digit “µ‑state” that records, for each column,
 *  whether the previous row had a 1 and whether that 1 already found a
 *  neighbour.  Each column thus has three possible values:
 *     0 – previous row had a 0
 *     1 – previous row had a 1 that has not yet found a neighbour
 *     2 – previous row had a 1 that already found a neighbour.
 *  The state space has 3⁴ = 81 elements.  Transition from one state to the
 *  next is performed for each of the 16 possible current‑row patterns.
 *  After processing n rows the number of valid arrays is the sum of
 *  counts in all states that contain no unsatisfied 1‑cells (i.e. no
 *  “1” among the four digits).
 *
 *  The implementation memoises results for all n up to the largest
 *  requested, so subsequent calls are O(n) but only once per n.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define STATE_COUNT 81

/* decode[st][c]  = 0,1,2 as described above */
static int decode[STATE_COUNT][4];
/* unsatisfied[st] = true if some digit of st equals 1 */
static bool unsatisfied[STATE_COUNT];
/* cache of a(n) values */
static seq_t *a_cache = NULL;
static idx_t cache_len = 0;
/* dp arrays */
static seq_t *dp_prev = NULL;
static seq_t *dp_next = NULL;
/* one‑shot initialisation */
static bool init_done = false;

static void init_tables(void)
{
    for (int s = 0; s < STATE_COUNT; ++s) {
        int tmp = s;
        unsatisfied[s] = false;
        for (int c = 0; c < 4; ++c) {
            int d = tmp % 3;
            tmp /= 3;
            decode[s][c] = d;
            if (d == 1) unsatisfied[s] = true;
        }
    }
    /* allocate dp arrays once */
    dp_prev = (seq_t *)calloc(STATE_COUNT, sizeof(seq_t));
    dp_next = (seq_t *)calloc(STATE_COUNT, sizeof(seq_t));
    /* memoise first a(1) onwards */
    a_cache = (seq_t *)calloc(1, sizeof(seq_t)); /* index 0 unused */
    cache_len = 0;
    init_done = true;
}

static inline int encode_state(const int st[4])
{
    int s = 0, power = 1;
    for (int c = 0; c < 4; ++c) {
        s += st[c] * power;
        power *= 3;
    }
    return s;
}

/* Ensure a_cache contains values up to n; compute them if needed. */
static void grow_cache_to(idx_t n)
{
    if (!init_done) init_tables();
    if (n <= cache_len) return;

    /* enlarge a_cache to hold n (1‑based) */
    seq_t *tmp = (seq_t *)realloc(a_cache, (n + 1) * sizeof(seq_t));
    if (!tmp) return; /* allocation failure: leave cache incomplete */
    a_cache = tmp;
    /* initialise new entries */
    for (idx_t i = cache_len + 1; i <= n; ++i) a_cache[i] = 0;

    /* initialise dp for 0 rows : state 0 has count 1 */
    if (cache_len == 0) {
        for (int s = 0; s < STATE_COUNT; ++s) dp_prev[s] = 0;
        dp_prev[0] = (seq_t)1;
    }

    for (idx_t rows = cache_len; rows < n; ++rows) {
        /* reset next table */
        for (int s = 0; s < STATE_COUNT; ++s) dp_next[s] = 0;
        for (int pr = 0; pr < STATE_COUNT; ++pr) {
            if (dp_prev[pr] == 0) continue;
            /* prev pattern bits: >0 indicates a 1 in that column */
            bool prev_bits[4];
            for (int c = 0; c < 4; ++c) prev_bits[c] = (decode[pr][c] != 0);
            for (int cur = 0; cur < 16; ++cur) {
                bool ok = true;
                /* 1. all unsatisfied cells in previous row must get a neighbour */
                for (int c = 0; c < 4; ++c) {
                    if (decode[pr][c] == 1) {
                        bool neigh = false;
                        if (c > 0 && (cur & (1 << (c - 1)))) neigh = true;
                        if (cur & (1 << c)) neigh = true;
                        if (c < 3 && (cur & (1 << (c + 1)))) neigh = true;
                        if (!neigh) { ok = false; break; }
                    }
                }
                if (!ok) continue;

                /* 2. compute new state for current row */
                int newst[4] = {0, 0, 0, 0};
                for (int c = 0; c < 4; ++c) {
                    if (cur & (1 << c)) {
                        bool neigh = false;
                        /* horizontal neighbours */
                        if (c > 0 && (cur & (1 << (c - 1)))) neigh = true;
                        if (c < 3 && (cur & (1 << (c + 1)))) neigh = true;
                        /* vertical/diagonal neighbours from previous row */
                        for (int dc = -1; dc <= 1; ++dc) {
                            int pc = c + dc;
                            if (pc >= 0 && pc < 4 && prev_bits[pc]) neigh = true;
                        }
                        newst[c] = neigh ? 2 : 1;
                    } else {
                        newst[c] = 0;
                    }
                }
                int ns = encode_state(newst);
                dp_next[ns] += dp_prev[pr];
            }
        }
        /* swap tables */
        seq_t *tmp_dp = dp_prev; dp_prev = dp_next; dp_next = tmp_dp;

        /* compute a[rows+1] (number of rows processed = rows+1) */
        idx_t idx = rows + 1;
        seq_t sum = 0;
        for (int s = 0; s < STATE_COUNT; ++s) {
            if (!unsatisfied[s]) sum += dp_prev[s];
        }
        a_cache[idx] = sum;
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* defined behaviour below offset */
    grow_cache_to(n);
    return a_cache[n];
}
