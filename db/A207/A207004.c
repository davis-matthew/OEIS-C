/*
 * A207004  Number of 7Xn 0..1 arrays avoiding 0 1 0 horizontally and 0 1 1
 *          vertically.
 * https://oeis.org/A207004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by a transfer‑matrix DP over allowed column pairs
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool initialized = false;
static int V;                 /* number of admissible column patterns (54) */
static uint8_t *col_list;      /* list of bitmasks for admissible columns          */
static int S;                 /* number of pairs (V*V)                           */
static uint16_t *pair_to;     /* concatenated list of successor states           */
static uint32_t *pair_off;    /* offset table for the above list (size S+1)      */
static uint32_t T;            /* total number of transitions                      */
static seq_t *dp_curr;        /* DP array for current step                        */
static seq_t *dp_next;        /* DP array for next step                           */

/* Test whether a single column satisfies the vertical restriction 0‑1‑1. */
static inline bool valid_col(uint8_t mask) {
    for (int r = 0; r <= 4; ++r) {
        if (((mask >> r) & 1) == 0 &&
            ((mask >> (r + 1)) & 1) == 1 &&
            ((mask >> (r + 2)) & 1) == 1) {
            return false;
        }
    }
    return true;
}

/* Test whether a triple of columns satisfies the horizontal restriction
   0‑1‑0 per row. */
static inline bool valid_triple(uint8_t a, uint8_t b, uint8_t c) {
    for (int r = 0; r < 7; ++r) {
        if (((a >> r) & 1) == 0 &&
            ((b >> r) & 1) == 1 &&
            ((c >> r) & 1) == 0) {
            return false;
        }
    }
    return true;
}

/* One‑time static initialisation of all data structures. */
static void init(void) {
    if (initialized) return;

    /* ---- 1. Build list of admissible columns ---- */
    V = 0;
    for (int m = 0; m < 128; ++m) {
        if (valid_col((uint8_t)m)) ++V;
    }
    col_list = (uint8_t *)malloc(V * sizeof(uint8_t));
    int idx = 0;
    for (int m = 0; m < 128; ++m) {
        if (valid_col((uint8_t)m)) col_list[idx++] = (uint8_t)m;
    }

    /* ---- 2. Build transition tables for pairs of columns ---- */
    S = V * V;
    uint8_t *counts = (uint8_t *)malloc(S * sizeof(uint8_t));
    pair_off = (uint32_t *)malloc((S + 1) * sizeof(uint32_t));
    pair_off[0] = 0;

    /* 2a. Count successors for each state */
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            uint8_t cnt = 0;
            for (int k = 0; k < V; ++k) {
                if (valid_triple(col_list[i], col_list[j], col_list[k])) {
                    ++cnt;
                }
            }
            int id = i * V + j;
            counts[id] = cnt;
            pair_off[id + 1] = pair_off[id] + cnt;
        }
    }
    T = pair_off[S];

    /* 2b. Build list of successor state indices */
    pair_to = (uint16_t *)malloc(T * sizeof(uint16_t));
    uint32_t pos = 0;
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            for (int k = 0; k < V; ++k) {
                if (valid_triple(col_list[i], col_list[j], col_list[k])) {
                    pair_to[pos++] = (uint16_t)(j * V + k);
                }
            }
        }
    }
    free(counts);

    /* ---- 3. Allocate DP arrays ---- */
    dp_curr = (seq_t *)calloc(S, sizeof(seq_t));
    dp_next = (seq_t *)calloc(S, sizeof(seq_t));

    initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n) {
    init();

    if (n == 0) return 0;          /* offset below defined; undefined behaviour */
    if (n == 1) return (seq_t)V;    /* single column: any admissible column */
    if (n == 2) {
        seq_t v = (seq_t)V;
        return v * v;              /* two columns: no horizontal restriction */
    }

    /* Initialise DP for sequences of length 2: each admissible pair counts once */
    for (int s = 0; s < S; ++s) dp_curr[s] = 1;

    /* DP for length >= 3 */
    for (idx_t step = 3; step <= n; ++step) {
        /* reset dp_next to zero */
        for (int s = 0; s < S; ++s) dp_next[s] = 0;

        for (int s = 0; s < S; ++s) {
            seq_t val = dp_curr[s];
            if (val == 0) continue;

            uint32_t start = pair_off[s];
            uint32_t end   = pair_off[s + 1];
            for (uint32_t p = start; p < end; ++p) {
                uint16_t t = pair_to[p];
                dp_next[t] += val;
            }
        }

        /* swap dp arrays for next iteration */
        seq_t *tmp = dp_curr;
        dp_curr = dp_next;
        dp_next = tmp;
    }

    /* Sum all states to obtain the total number of n‑column arrays */
    seq_t result = 0;
    for (int s = 0; s < S; ++s) result += dp_curr[s];
    return result;
}
