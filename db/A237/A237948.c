/*
 * A237948  Number of (n+1)X(3+1) 0..3 arrays with the maximum plus the upper
 *          median plus the lower median of every 2X2 subblock differing from
 *          its horizontal and vertical neighbors by exactly one.
 * https://oeis.org/A237948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)x4 0..3 arrays satisfying the *
 *            2x2‑subblock condition. Computed by dynamic programming over
 *            rows
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 14, u128 n <= 31,
 *            big any n
 * Helpers:   build_row_values, top3sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>  /* for abs */
#include "../utils/build_row_values.h"
#include "../utils/top3sum.h"

/* Precomputed data structures for the DP */
typedef struct {
    uint16_t nextRow;   /* next row index 0..255 */
    uint8_t  newTrip;   /* index of new triple 0..35 */
} transition_t;

static bool          horiz[256][256];              /* horizontal validity */
static uint8_t       triple_vals[256][256][3];      /* triple (S0,S1,S2) */
static int8_t        triple_idx[256][256];          /* index 0..35 or -1 */
static uint8_t       triple_tab[36][3];             /* triple values by index */
static int           trip_start[36];                /* not used, kept for compatibility */
static size_t        state_start[256*36 + 1];
static transition_t *transitions = NULL;           /* all transitions */
static size_t        total_transitions = 0;
static bool          precomputed = false;

/* Map a legal triple to a unique index 0..35 */
static void build_triple_table(void)
{
    int map[10][10][10];
    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j)
            for (int k = 0; k < 10; ++k)
                map[i][j][k] = -1;
    int idx = 0;
    for (uint16_t a = 0; a < 256; ++a) {
        for (uint16_t b = 0; b < 256; ++b) {
            if (!horiz[a][b]) continue;
            int s0 = triple_vals[a][b][0];
            int s1 = triple_vals[a][b][1];
            int s2 = triple_vals[a][b][2];
            if (map[s0][s1][s2] == -1) {
                map[s0][s1][s2] = idx;
                triple_tab[idx][0] = (uint8_t)s0;
                triple_tab[idx][1] = (uint8_t)s1;
                triple_tab[idx][2] = (uint8_t)s2;
                ++idx;
            }
        }
    }
    for (uint16_t a = 0; a < 256; ++a) {
        for (uint16_t b = 0; b < 256; ++b) {
            if (horiz[a][b]) {
                int s0 = triple_vals[a][b][0];
                int s1 = triple_vals[a][b][1];
                int s2 = triple_vals[a][b][2];
                triple_idx[a][b] = (int8_t)map[s0][s1][s2];
            } else {
                triple_idx[a][b] = -1;
            }
        }
    }
}

/* Build transitions between DP states */
static void build_transitions(void)
{
    const int ST = 256*36;               /* number of states */

    size_t *cnt = (size_t*)calloc(ST, sizeof(size_t));
    for (uint16_t r = 0; r < 256; ++r) {
        for (uint8_t t_old = 0; t_old < 36; ++t_old) {
            size_t sidx = (size_t)r*36 + t_old;
            for (uint16_t c = 0; c < 256; ++c) {
                if (!horiz[r][c]) continue;
                int8_t t_new = triple_idx[r][c];
                if (t_new == -1) continue;
                bool ok = true;
                for (int j = 0; j < 3 && ok; ++j) {
                    uint8_t a = triple_tab[t_old][j];
                    uint8_t b = triple_tab[t_new][j];
                    if (a > b) ok = (a - b == 1);
                    else ok = (b - a == 1);
                }
                if (ok) ++cnt[sidx];
            }
        }
    }

    state_start[0] = 0;
    for (size_t i = 0; i < ST; ++i)
        state_start[i+1] = state_start[i] + cnt[i];

    total_transitions = state_start[ST];
    transitions = (transition_t*)malloc(total_transitions * sizeof(transition_t));

    size_t *filled = (size_t*)calloc(ST, sizeof(size_t));
    for (uint16_t r = 0; r < 256; ++r) {
        for (uint8_t t_old = 0; t_old < 36; ++t_old) {
            size_t sidx = (size_t)r*36 + t_old;
            size_t pos = state_start[sidx];
            for (uint16_t c = 0; c < 256; ++c) {
                if (!horiz[r][c]) continue;
                int8_t t_new = triple_idx[r][c];
                if (t_new == -1) continue;
                bool ok = true;
                for (int j = 0; j < 3 && ok; ++j) {
                    uint8_t a = triple_tab[t_old][j];
                    uint8_t b = triple_tab[t_new][j];
                    if (a > b) ok = (a - b == 1);
                    else ok = (b - a == 1);
                }
                if (!ok) continue;
                transitions[pos + filled[sidx]].nextRow = (uint16_t)c;
                transitions[pos + filled[sidx]].newTrip  = (uint8_t)t_new;
                ++filled[sidx];
            }
        }
    }

    free(cnt);
    free(filled);
}

/* Initialise all tables once */
static void init_precomputation(void)
{
    if (precomputed) return;
    precomputed = true;

    uint8_t row_vals[256][4];
    build_row_values(row_vals);

    for (uint16_t a = 0; a < 256; ++a) {
        for (uint16_t b = 0; b < 256; ++b) {
            uint8_t sa0 = top3sum(row_vals[a][0], row_vals[a][1],
                                   row_vals[b][0], row_vals[b][1]);
            uint8_t sa1 = top3sum(row_vals[a][1], row_vals[a][2],
                                   row_vals[b][1], row_vals[b][2]);
            uint8_t sa2 = top3sum(row_vals[a][2], row_vals[a][3],
                                   row_vals[b][2], row_vals[b][3]);

            triple_vals[a][b][0] = sa0;
            triple_vals[a][b][1] = sa1;
            triple_vals[a][b][2] = sa2;

            horiz[a][b] = (abs((int)sa0 - (int)sa1) == 1 &&
                           abs((int)sa1 - (int)sa2) == 1);
        }
    }

    build_triple_table();
    build_transitions();
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* the sequence starts at n=1 */

    init_precomputation();

    const size_t ST = 256*36;              /* number of DP states */
    seq_t *dp_cur  = (seq_t*)calloc(ST, sizeof(seq_t));
    seq_t *dp_next = (seq_t*)calloc(ST, sizeof(seq_t));

    /* Initialise with the two first rows */
    for (uint16_t a = 0; a < 256; ++a) {
        for (uint16_t b = 0; b < 256; ++b) {
            if (!horiz[a][b]) continue;
            int8_t t = triple_idx[a][b];
            if (t == -1) continue;
            size_t id = ((size_t)b * 36) + t;
            dp_cur[id] += (seq_t)1;
        }
    }

    /* Extend rows for n-1 additional rows (total n+1 rows) */
    for (idx_t step = 1; step <= n-1; ++step) {
        memset(dp_next, 0, ST * sizeof(seq_t));
        for (size_t sid = 0; sid < ST; ++sid) {
            seq_t cnt = dp_cur[sid];
            if (cnt == (seq_t)0) continue;
            size_t start = state_start[sid];
            size_t end   = state_start[sid+1];
            for (size_t p = start; p < end; ++p) {
                transition_t tr = transitions[p];
                size_t nid = ((size_t)tr.nextRow * 36) + tr.newTrip;
                dp_next[nid] += cnt;
            }
        }
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* Sum all final states */
    seq_t result = (seq_t)0;
    for (size_t sid = 0; sid < ST; ++sid)
        result += dp_cur[sid];

    free(dp_cur);
    free(dp_next);
    return result;
}
