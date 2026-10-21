/*
 * A220643  Number of ways to reciprocally link elements of an n X 7 array
 *          either to themselves or to exactly one king-move neighbor.
 * https://oeis.org/A220643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A220643: Number of ways to reciprocally link elements of an
   n X 7 array either to themselves or to exactly one king‑move neighbor.
   The following implementation computes the number of perfect
   matchings in the 7‑column king graph for an n‑row board by
   dynamic programming over the 128 possible row states.
*/

#include <stdint.h>
#include <stdbool.h>

/* Transition entry: next state and multiplicity */
typedef struct {
    unsigned char to;     /* next row state (0..127) */
    unsigned int weight;  /* number of ways leading to 'to' */
} transition_t;

#define STATES 128          /* 2^7 possible states */
#define ROWS   7           /* number of columns */

/* Transition table and counters */
static transition_t trans[STATES][STATES]; /* second dimension enough for all Unique 'to' */
static unsigned char trans_cnt[STATES];

/* Counter for number of ways to reach each next state while building transitions */
static unsigned int count_next[STATES];

/* Recursive helper to generate all transitions from a starting state.
   'used' bits are those already occupied in the current row (from previous row matchings).
   'nextst' bits will be set for cells matched downward from the current row.
   'pos' is the next column to consider (0 .. ROWS). */
static void dfs_generate(unsigned char used, unsigned char nextst, unsigned char pos)
{
    /* Skip already used cells */
    while (pos < ROWS && (used & (1 << pos))) {
        pos++;
    }
    if (pos == ROWS) {
        /* All columns processed: record the resulting next state */
        count_next[nextst]++;
        return;
    }

    /* Option 1: leave cell as a self-loop */
    dfs_generate(used | (1 << pos), nextst, pos + 1);

    /* Option 2: match horizontally with right neighbor */
    if (pos + 1 < ROWS && !(used & (1 << (pos + 1)))) {
        dfs_generate(used | (1 << pos) | (1 << (pos + 1)), nextst, pos + 2);
    }

    /* Option 3: match vertically downward */
    if (!(nextst & (1 << pos))) {
        dfs_generate(used | (1 << pos), nextst | (1 << pos), pos + 1);
    }

    /* Option 4: match diagonally down-left */
    if (pos > 0 && !(nextst & (1 << (pos - 1)))) {
        dfs_generate(used | (1 << pos), nextst | (1 << (pos - 1)), pos + 1);
    }

    /* Option 5: match diagonally down-right */
    if (pos + 1 < ROWS && !(nextst & (1 << (pos + 1)))) {
        dfs_generate(used | (1 << pos), nextst | (1 << (pos + 1)), pos + 1);
    }
}

/* Build the transition table once */
static void init_transitions(void)
{
    for (unsigned char s = 0; s < STATES; ++s) {
        /* reset counts for this start state */
        for (int i = 0; i < STATES; ++i) count_next[i] = 0;

        /* generate transitions starting with 's' occupied cells */
        dfs_generate(s, 0, 0);

        /* compact non-zero counts into the transition list */
        unsigned char ctr = 0;
        for (unsigned char to = 0; to < STATES; ++to) {
            if (count_next[to] != 0) {
                trans[s][ctr].to = to;
                trans[s][ctr].weight = count_next[to];
                ctr++;
            }
        }
        trans_cnt[s] = ctr;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_transitions();
        initialized = true;
    }

    /* DP state arrays */
    seq_t cur[STATES];
    seq_t nxt[STATES];

    for (int i = 0; i < STATES; ++i) cur[i] = 0;
    cur[0] = (seq_t)1;            /* a(0) = 1 */

    /* Trivial case */
    if (n == 0) return cur[0];

    for (idx_t row = 0; row < n; ++row) {
        for (int i = 0; i < STATES; ++i) nxt[i] = 0;

        for (unsigned char prev = 0; prev < STATES; ++prev) {
            seq_t val = cur[prev];
            if (val == 0) continue;

            unsigned char tcnt = trans_cnt[prev];
            for (unsigned char t = 0; t < tcnt; ++t) {
                unsigned char to   = trans[prev][t].to;
                unsigned int  w    = trans[prev][t].weight;
                /* nxt[to] += val * w */
                nxt[to] = nxt[to] + (val * (seq_t)w);
            }
        }

        /* advance to next row */
        for (int i = 0; i < STATES; ++i) cur[i] = nxt[i];
    }

    return cur[0];
}
