/*
 * A205070  Number of (n+1)X7 0..1 arrays with the number of rightwards and
 *          downwards edge increases in each 2X2 subblock differing from the
 *          number in all its horizontal and vertical neighbors.
 * https://oeis.org/A205070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence for n > 41 a(n) = 3 a(n‑2)
 *            - 2 a(n‑6) - 3 a(n‑8) - 3 a(n‑10) - 2 a(n‑12) + a(n‑14) + 2
 *            a(n‑16) + 9 a(n‑18) + 14 a(n‑20) + 17 a(n‑22) + 18 a(n‑24) + 17
 *            a(n‑26) + 15 a(n‑28) + 10 a(n‑30) + 6 a(n‑32) + 3 a(n‑34) +
 *            a(n‑36) Initial values for n = 1 … 41 are pre‑com...
 * Range:     u8 none, u16 n <= 15, u32 n <= 44, u64 n <= 102, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------- */
/*  Graph construction and initial 41 terms                       */
/* ------------------------------------------------------------- */

#define MAX_ROWS 128                     /* 7‑bit rows: 0 … 127 */
#define MAX_PAIRS (MAX_ROWS * MAX_ROWS) /* 16384 pair identifiers */

/* states for all pairs (row1,row2) */
static uint8_t     valid_pair[MAX_PAIRS];          /* 1 = admissible pair */
static unsigned char inc_vec[MAX_PAIRS][6];        /* horizontal/vertical increments per 2×2 block */
static uint16_t    adj_next[MAX_PAIRS][128];       /* adjacency list of next row ids (≤128) */
static uint8_t     adj_len[MAX_PAIRS];             /* number of successors for each pair */
static bool        graph_built = false;

/* initial 41 terms, 1‑based.  Element 0 is unused. */
static seq_t init_vals[42];
static bool   init_done = false;

/* build graph and compute a(1)…a(41) by DP */
static void build_graph_and_init(void)
{
    if (graph_built) return;
    graph_built = true;

    /* ---------- 1. Identify admissible pairs and increment vectors ---------- */
    for (int r1 = 0; r1 < MAX_ROWS; ++r1) {
        for (int r2 = 0; r2 < MAX_ROWS; ++r2) {
            int id = (r1 << 7) | r2;          /* unique id for the pair (r1, r2) */
            unsigned char v[6];
            bool ok = true;

            for (int j = 0; j < 6; ++j) {
                int a0 = (r1 >> j)     & 1;   /* left‑top of the 2×2 block */
                int a1 = (r1 >> (j+1)) & 1;   /* right‑top */
                int b0 = (r2 >> j)     & 1;   /* left‑bottom */
                int b1 = (r2 >> (j+1)) & 1;   /* right‑bottom */

                int inc = 0;
                if (a0 == 0 && a1 == 1) inc++;   /* horizontal top edge */
                if (b0 == 0 && b1 == 1) inc++;   /* horizontal bottom edge */
                if (a0 == 0 && b0 == 1) inc++;   /* vertical left edge */
                if (a1 == 0 && b1 == 1) inc++;   /* vertical right edge */
                v[j] = (unsigned char)inc;

                /* horizontal neighbour rule: adjacent 2×2 blocks must differ */
                if (j > 0 && v[j] == v[j-1]) { ok = false; break; }
            }
            valid_pair[id] = ok ? 1 : 0;
            for (int k = 0; k < 6; ++k) inc_vec[id][k] = ok ? v[k] : 0;
        }
    }

    /* ---------- 2. build adjacency lists (vertical neighbour rule) ---------- */
    for (int id = 0; id < MAX_PAIRS; ++id) {
        if (!valid_pair[id]) continue;                     /* first pair must be admissible */
        int r1 = id >> 7;                                 /* first row of current pair */
        int r2 = id & 127;                                /* second row of current pair */
        uint8_t len = 0;

        for (int r3 = 0; r3 < MAX_ROWS; ++r3) {           /* try every possible next row */
            int next_id = (r2 << 7) | r3;
            if (!valid_pair[next_id]) continue;           /* next pair must be admissible */

            bool ok = true;
            for (int k = 0; k < 6; ++k) {
                if (inc_vec[id][k] == inc_vec[next_id][k]) { ok = false; break; }
            }
            if (!ok) continue;
            adj_next[id][len++] = (uint16_t)next_id;
        }
        adj_len[id] = len;
    }

    /* ---------- 3. dynamic programming to obtain the first 41 terms ---------- */
    seq_t curr[MAX_PAIRS];
    seq_t next[MAX_PAIRS];

    /* a(1) : all admissible single pairs contribute 1 */
    seq_t sum = 0;
    for (int id = 0; id < MAX_PAIRS; ++id) {
        curr[id] = valid_pair[id] ? (seq_t)1 : (seq_t)0;
        sum += curr[id];
    }
    init_vals[1] = sum;

    for (idx_t step = 2; step <= 41; ++step) {
        /* zero the next array */
        for (int id = 0; id < MAX_PAIRS; ++id) next[id] = (seq_t)0;

        /* propagate counts */
        for (int id = 0; id < MAX_PAIRS; ++id) {
            seq_t val = curr[id];
            if (val == (seq_t)0) continue;
            uint8_t len = adj_len[id];
            for (uint8_t e = 0; e < len; ++e) {
                int nid = adj_next[id][e];
                next[nid] += val;
            }
        }
        /* swap buffers */
        for (int id = 0; id < MAX_PAIRS; ++id) curr[id] = next[id];

        /* sum current counts to get a(step) */
        sum = 0;
        for (int id = 0; id < MAX_PAIRS; ++id) sum += curr[id];
        init_vals[step] = sum;
    }

    init_done = true;
}

/* ------------------------------------------------------------- */
/*  Public function: a(idx_t n)                                  */
/* ------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* make sure the initial terms are available */
    if (!init_done) build_graph_and_init();

    if (n <= 41) return init_vals[(int)n];

    /* ring buffer: we need values up to 36 places back */
    static seq_t buf[37];
    for (idx_t i = 1; i <= 41; ++i) buf[i % 37] = init_vals[(int)i];

    for (idx_t k = 42; k <= n; ++k) {
        seq_t val  = 3 * buf[(k - 2) % 37];
        val       += buf[(k - 14) % 37];
        val       += 2 * buf[(k - 16) % 37];
        val       += 9 * buf[(k - 18) % 37];
        val       += 14 * buf[(k - 20) % 37];
        val       += 17 * buf[(k - 22) % 37];
        val       += 18 * buf[(k - 24) % 37];
        val       += 17 * buf[(k - 26) % 37];
        val       += 15 * buf[(k - 28) % 37];
        val       += 10 * buf[(k - 30) % 37];
        val       += 6  * buf[(k - 32) % 37];
        val       += 3  * buf[(k - 34) % 37];
        val       += buf[(k - 36) % 37];

        seq_t neg = 2 * buf[(k - 6) % 37];
        neg       += 3 * buf[(k - 8) % 37];
        neg       += 3 * buf[(k - 10) % 37];
        neg       += 2 * buf[(k - 12) % 37];

        val -= neg;
        buf[k % 37] = val;
    }

    return buf[n % 37];
}
