/*
 * A233889  Number of (n+1)X(5+1) 0..6 arrays with every 2X2 subblock having
 *          the sum of the squares of the edge differences equal to 30, and no
 *          two adjacent values equal.
 * https://oeis.org/A233889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A233889:
   a(n) = number of (n+1)×6 arrays with entries 0..6 such that
   - no two horizontally or vertically adjacent entries are equal,
   - for every 2×2 subblock the sum of the squares of its edge
     differences equals 30. */
      
#include <stdint.h>

typedef uint64_t idx_t;  /* provided by the build environment */

/* ---- static data ----------------------------------------------------- */
#define MAX_STATES 54432          /* number of admissible 6‑tuples */
#define MAX_EDGES 20000          /* more than enough for all transitions   */
#define NUM_DIGITS 6
#define BASE 7

static struct Edge {
    int from;
    int to;
} edges[MAX_EDGES];
static int edge_count;

static uint8_t state_vals[MAX_STATES][NUM_DIGITS];
static int state_count;

static int pow7[NUM_DIGITS];
static int key_to_state[BASE * BASE * BASE * BASE * BASE * BASE]; /* 7^6 */

static int built = 0;

/* ---- recursive generator for transitions -------------------------------- */
static void dfs_transition(int u, int pos, const uint8_t *s, uint8_t *t)
{
    if (pos == NUM_DIGITS) {
        int key = 0;
        for (int i = 0; i < NUM_DIGITS; ++i)
            key += t[i] * pow7[i];
        int v = key_to_state[key];
        if (v >= 0) {
            edges[edge_count].from = u;
            edges[edge_count].to   = v;
            ++edge_count;
        }
        return;
    }
    for (int val = 0; val < BASE; ++val) {
        if (val == s[pos])          /* vertical adjacency      */
            continue;
        if (pos > 0) {
            if (val == t[pos-1])    /* horizontal adjacency    */
                continue;
            /* 2×2 constraint between columns pos‑1 and pos */
            int a = s[pos-1], b = s[pos];
            int c = t[pos-1], d = val;
            int d1 = a - b, d2 = c - d, d3 = a - c, d4 = b - d;
            int sumsq = d1*d1 + d2*d2 + d3*d3 + d4*d4;
            if (sumsq != 30)
                continue;
        }
        t[pos] = val;
        dfs_transition(u, pos+1, s, t);
    }
}

/* ---- build all states and transition edges ---------------------------- */
static void build_data(void)
{
    /* pre‑compute powers of 7 */
    pow7[0] = 1;
    for (int i = 1; i < NUM_DIGITS; ++i)
        pow7[i] = pow7[i-1] * BASE;

    /* initialise mapping to –1 */
    for (int i = 0; i < BASE*BASE*BASE*BASE*BASE*BASE; ++i)
        key_to_state[i] = -1;

    /* enumerate all 6‑tuples, keep those with no equal neighbours */
    state_count = 0;
    for (int key = 0; key < BASE*BASE*BASE*BASE*BASE*BASE; ++key) {
        int tmp = key;
        uint8_t v[NUM_DIGITS];
        for (int i = 0; i < NUM_DIGITS; ++i)
            v[i] = tmp % BASE, tmp /= BASE;

        int ok = 1;
        for (int i = 0; i < NUM_DIGITS-1; ++i)
            if (v[i] == v[i+1]) { ok = 0; break; }
        if (!ok) continue;

        key_to_state[key] = state_count;
        for (int i = 0; i < NUM_DIGITS; ++i)
            state_vals[state_count][i] = v[i];
        ++state_count;
    }

    /* generate all admissible transitions */
    uint8_t t[NUM_DIGITS];
    edge_count = 0;
    for (int u = 0; u < state_count; ++u)
        dfs_transition(u, 0, state_vals[u], t);
}

/* ---- main sequence function ------------------------------------------- */
extern seq_t a(idx_t n)
{
    /* build data only once */
    if (!built) { build_data(); built = 1; }

    /* dynamic programming over rows */
    static seq_t cur_arr[MAX_STATES];
    static seq_t next_arr[MAX_STATES];
    seq_t *cur = cur_arr, *next = next_arr;

    /* initialisation: one way to start with any admissible row */
    for (int i = 0; i < state_count; ++i)
        cur[i] = 1;

    /* iterate over (n) row spans */
    for (idx_t step = 0; step < n; ++step) {
        /* zero next */
        for (int i = 0; i < state_count; ++i)
            next[i] = 0;

        /* propagate along all edges */
        for (int e = 0; e < edge_count; ++e) {
            int from = edges[e].from;
            int to   = edges[e].to;
            next[to] += cur[from];
        }

        /* swap arrays for next iteration */
        seq_t *tmp = cur; cur = next; next = tmp;
    }

    /* sum of all final states gives the required count */
    seq_t total = 0;
    for (int i = 0; i < state_count; ++i)
        total += cur[i];

    return total;
}
