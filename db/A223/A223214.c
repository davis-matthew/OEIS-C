/*
 * A223214  3X3X3 triangular graph coloring a rectangular array: number of nX4
 *          0..5 arrays where 0..5 label nodes of a graph with edges 0,1 0,2
 *          1,2 1,3 1,4 2,4 3,4 2,5 4,5 and every array movement to a
 *          horizontal or vertical neighbor moves along an edge of this graph.
 * https://oeis.org/A223214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 36*a(n-1) +219*a(n-2) -5538*a(n-3) +3051*a(n-4)
 *            +141678*a(n-5) -180657*a(n-6) -980802*a(n-7) +1136403*a(n-8)
 *            +1765044*a(n-9) -1679697*a(n-10) -991440*a(n-11) +763668*a(n-12)
 *            +60264*a(n-13) -77760*a(n-14) +7776*a(n-15) with a(1)=192,
 *            a(2)=6642, a(3)=254694, a(4)=9640008, a(5)=367156350, a(6...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* Edge representation: undirected graph over colors 0..5 */
static bool edges[6][6] = {0};

/* List of valid 4‑tuples (states) that respect horizontal edges.
   Each state is encoded as a base‑6 integer 0..1295.           */
static int state_digits[1296][4];
static int state_code[1296];
static int num_states = 0;

/* Vertical adjacency lists: state i can be followed by state j. */
static int adj[1296][256];
static int adj_count[1296];

/* Recurrence coefficients (int64_t) for a(n) = Σ coeff[k-1]*a(n-k) */
static const int coeff[15] = {
    36, 219, -5538, 3051, 141678,
    -180657, -980802, 1136403, 1765044,
    -1679697, -991440, 763668, 60264,
    -77760, 7776
};

/* Initial terms a(1)..a(15) */
static seq_t initVals[16] = {0};

static bool initialized = false;

/* ------------------------------------------------------------ */
/* Build graph, enumerate states, compute first 15 terms by DP */
/* ------------------------------------------------------------ */
static void init_sequence(void)
{
    if (initialized) return;

    /* Build edge table */
#define ADD(a,b) (edges[(a)][(b)] = edges[(b)][(a)] = true)
    ADD(0,1); ADD(0,2); ADD(1,2); ADD(1,3); ADD(1,4);
    ADD(2,4); ADD(3,4); ADD(2,5); ADD(4,5);
#undef ADD

    /* Enumerate all 4‑tuples with horizontal adjacency */
    num_states = 0;
    for (int code = 0; code < 1296; ++code) {
        int tmp = code;
        bool ok = true;
        for (int j = 0; j < 4; ++j) {
            int d = tmp % 6;
            tmp /= 6;
            state_digits[num_states][j] = d;
            if (j > 0 && !edges[state_digits[num_states][j-1]][d]) ok = false;
        }
        if (ok) {
            state_code[num_states] = code;
            ++num_states;
        }
    }

    /* Build vertical adjacency lists */
    for (int i = 0; i < num_states; ++i) {
        adj_count[i] = 0;
        for (int j = 0; j < num_states; ++j) {
            bool ok = true;
            for (int k = 0; k < 4; ++k) {
                if (!edges[state_digits[i][k]][state_digits[j][k]]) { ok = false; break; }
            }
            if (ok) adj[i][adj_count[i]++] = j;
        }
    }

    /* DP to compute a(1)..a(15) */
    static seq_t dp_prev[1296];
    static seq_t dp_new[1296];
    memset(dp_prev, 0, sizeof(dp_prev));
    for (int s = 0; s < num_states; ++s) dp_prev[s] = (seq_t)1;

    /* a(1) */
    seq_t sum = 0;
    for (int s = 0; s < num_states; ++s) sum += dp_prev[s];
    initVals[1] = sum;

    for (int row = 2; row <= 15; ++row) {
        memset(dp_new, 0, sizeof(dp_new));
        for (int i = 0; i < num_states; ++i) {
            seq_t val = dp_prev[i];
            if (!val) continue;
            for (int j = 0; j < adj_count[i]; ++j) {
                int nb = adj[i][j];
                dp_new[nb] += val;
            }
        }
        sum = 0;
        for (int s = 0; s < num_states; ++s) sum += dp_new[s];
        initVals[row] = sum;
        memcpy(dp_prev, dp_new, sizeof(dp_prev));
    }

    initialized = true;
}

/* ------------------------------------------------------------ */
/* Main sequence routine */
seq_t a(idx_t n)
{
    if (!initialized) init_sequence();

    if (n <= 15u) return initVals[(int)n];

    /* Sliding window: last[0] = a(n-15), ..., last[14] = a(n-1) */
    seq_t last[15];
    for (int i = 0; i < 15; ++i) last[i] = initVals[i+1];

    seq_t result = 0;
    idx_t step;
    for (step = 16; step <= n; ++step) {
        seq_t pos = 0, neg = 0;
        for (int i = 0; i < 15; ++i) {
            int c = coeff[i];
            seq_t prod = last[14-i] * (seq_t)(c > 0 ? c : -c);
            if (c > 0) pos += prod;
            else      neg += prod;
        }
        result = pos - neg;               /* safe: pos >= neg for valid recurrence */
        /* shift window */
        for (int i = 0; i < 14; ++i) last[i] = last[i+1];
        last[14] = result;
    }
    return result;
}
