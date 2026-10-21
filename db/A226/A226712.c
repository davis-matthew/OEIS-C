/*
 * A226712  Number of nX3 (-1,0,1) arrays of determinants of 2X2 subblocks of
 *          some (n+1)X4 binary array.
 * https://oeis.org/A226712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   count of n‑length words over the 27‑symbol alphabet * generated
 *            by a directed graph whose vertices are the 16 binary * 4‑bit
 *            vectors. From a vertex a to b the edge label is the * 3‑tuple of
 *            determinants of the 2×2 subblocks of the 2×4 binary * block
 *            formed by rows a,b. The number a(n) is the tota...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define ALPHA 27          /* number of possible 3‑tuple symbols (-1,0,1)^3 */
#define ROWS 16           /* number of possible 4‑bit rows */
#define MAX_STATES (1U << ROWS)   /* 65536 */

/* Function signature supplied by the build system */
seq_t a(idx_t n);

/* ---------- internal static data ---------- */
static int *transitions = NULL;      /* flattened [[id][sym]] -> next_id or -1 */
static int state_id[MAX_STATES];    /* mask → DFA state id, -1 if unseen */
static int num_states = 0;           /* number of DFA states found */
static int start_state = 0;          /* id of the initial state (all rows) */

static seq_t *dp_cur = NULL;         /* DP arrays, allocated to MAX_STATES */
static seq_t *dp_next = NULL;
static bool initialized = false;

/* Build the DFA once */
static void build_automaton(void)
{
    /* 1. Precompute for each symbol s and each row a the set of rows b
       such that (a,b) produces symbol s.  Use a 16‑bit mask. */
    uint16_t next_mask[ALPHA][ROWS];
    memset(next_mask, 0, sizeof(next_mask));

    for (int a = 0; a < ROWS; ++a) {
        for (int b = 0; b < ROWS; ++b) {
            int v[3];
            for (int j = 0; j < 3; ++j) {
                int a_j  = (a >> j) & 1;
                int a_j1 = (a >> (j + 1)) & 1;
                int b_j1 = (b >> (j + 1)) & 1;
                int b_j  = (b >> j) & 1;
                v[j] = (a_j & b_j1) - (a_j1 & b_j);   /* -1,0,1 */
            }
            int s = ((v[0] + 1) * 9 + (v[1] + 1) * 3 + (v[2] + 1));  /* 0..26 */
            next_mask[s][a] |= (uint16_t)(1u << b);
        }
    }

    /* 2. Breadth‑first search over masks to discover DFA states.
       Use state_id[mask] to map mask → state id. */
    for (int i = 0; i < MAX_STATES; ++i) state_id[i] = -1;

    /* Allocate transition table for at most MAX_STATES states. */
    transitions = (int *)malloc(MAX_STATES * ALPHA * sizeof(int));
    for (int i = 0; i < MAX_STATES * ALPHA; ++i) transitions[i] = -1;

    uint16_t queue[MAX_STATES];
    size_t head = 0, tail = 0;

    uint16_t start_mask = 0xFFFF;    /* all 16 rows possible */
    state_id[start_mask] = 0;
    num_states = 1;
    start_state = 0;
    queue[tail++] = start_mask;

    while (head < tail) {
        uint16_t cur_mask = queue[head++];
        int cur_id = state_id[cur_mask];

        for (int s = 0; s < ALPHA; ++s) {
            uint16_t next_val = 0;
            /* Union of next rows for all a in cur_mask */
            for (int a = 0; a < ROWS; ++a) {
                if (cur_mask & (1u << a))
                    next_val |= next_mask[s][a];
            }
            if (next_val == 0) continue;   /* no transition for this symbol */

            int next_id;
            if (state_id[next_val] == -1) {
                next_id = num_states++;
                state_id[next_val] = next_id;
                queue[tail++] = next_val;
            } else {
                next_id = state_id[next_val];
            }
            transitions[cur_id * ALPHA + s] = next_id;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (!initialized) {
        build_automaton();
        /* Allocate DP arrays (fixed size MAX_STATES works for all cases). */
        dp_cur  = (seq_t *)malloc(MAX_STATES * sizeof(seq_t));
        dp_next = (seq_t *)malloc(MAX_STATES * sizeof(seq_t));
        initialized = true;
    }

    /* Zero current DP counts */
    for (int i = 0; i < num_states; ++i) dp_cur[i] = 0;
    dp_cur[start_state] = 1;   /* start from the universal set */

    for (idx_t step = 0; step < n; ++step) {
        /* Zero next counts */
        for (int i = 0; i < num_states; ++i) dp_next[i] = 0;

        for (int i = 0; i < num_states; ++i) {
            seq_t val = dp_cur[i];
            if (!val) continue;
            for (int s = 0; s < ALPHA; ++s) {
                int next_id = transitions[i * ALPHA + s];
                if (next_id < 0) continue;      /* dead transition */
                dp_next[next_id] += val;
            }
        }
        /* swap dp_cur and dp_next */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* Sum all counts to obtain total number of distinct arrays. */
    seq_t total = 0;
    for (int i = 0; i < num_states; ++i)
        total += dp_cur[i];
    return total;
}
