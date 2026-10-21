/*
 * A223776  Number of nX7 0..1 arrays with rows and columns unimodal and
 *          antidiagonals nondecreasing.
 * https://oeis.org/A223776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×7 0/1 arrays with each row and column 0*1*0*
 *            (unimodal) and each antidiagonal nondecreasing
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 26, u64 n <= 141, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Number of columns in the arrays. */
#define NUM_COLS   7

/* Total number of possible column-state configurations (statespace). */
#define NUM_STATES 2187          /* 3^7 */

/* Maximum number of distinct unimodal row patterns for 7 columns. */
#define MAX_PATTERNS 29

/* Transition table entry.  trans_count[s] gives the number of entries
   for a given starting state s. */
typedef struct {
    int next;  /* destination state */
    int cnt;   /* how many different row patterns lead to this state */
} transition_t;
static transition_t transitions[NUM_STATES][MAX_PATTERNS];
static unsigned char trans_count[NUM_STATES];

/* All 29 unimodal 7‑bit masks. */
static unsigned char unimodal_masks[MAX_PATTERNS];
static unsigned char unimodal_cnt = 0;

/* Base‑3 digit representation of each state.  state_digits[s][c] is the
   digit for column c (0‑based) in state s. */
static unsigned char state_digits[NUM_STATES][NUM_COLS];

/* Powers of 3: 3^j for j = 0 … 6.  Used for converting digits to a state integer. */
static const int pow3[NUM_COLS] = {1, 3, 9, 27, 81, 243, 729};

/* Check whether a 7‑bit mask represents a unimodal pattern 0*1*0*. */
static bool is_unimodal_mask(uint8_t mask)
{
    int state = 0;          /* 0 = zeros, 1 = ones, 2 = zeros after ones */
    for (int j = 0; j < NUM_COLS; ++j) {
        int bit = (mask >> j) & 1;
        if (state == 0) {
            if (bit == 1) state = 1;
        } else if (state == 1) {
            if (bit == 0) state = 2;
        } else {          /* state == 2 */
            if (bit == 1) return false;  /* cannot return to 1 */
        }
    }
    return true;
}

/* Initialise all static tables exactly once. */
static void init(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* Enumerate and store all unimodal 7‑bit masks. */
    for (uint16_t m = 0; m < (1U << NUM_COLS); ++m) {
        if (is_unimodal_mask((uint8_t)m)) {
            unimodal_masks[unimodal_cnt++] = (uint8_t)m;
        }
    }

    /* Compute the base‑3 representation for every state. */
    for (int s = 0; s < NUM_STATES; ++s) {
        int v = s;
        for (int j = 0; j < NUM_COLS; ++j) {
            state_digits[s][j] = (unsigned char)(v % 3);
            v /= 3;
        }
    }

    /* Build the transition table. */
    for (int s = 0; s < NUM_STATES; ++s) {
        trans_count[s] = 0;
        for (int pi = 0; pi < unimodal_cnt; ++pi) {
            uint8_t mask = unimodal_masks[pi];
            bool ok = true;

            /* Check column unimodality and antidiagonal constraints. */
            for (int j = 0; j < NUM_COLS; ++j) {
                int bit = (mask >> j) & 1;
                int d   = state_digits[s][j];

                /* Column unimodality: 1 after state 1 is allowed;
                   1 after state 2 is forbidden; 0 after state 1 becomes state 2. */
                if (d == 2 && bit == 1) { ok = false; break; }

                /* Antidiagonal nondecreasing: bit ≥ previous row's bit at (i-1,j+1). */
                if (j < NUM_COLS - 1) {
                    int prev_state = state_digits[s][j + 1];
                    int prev_bit   = (prev_state == 1) ? 1 : 0;
                    if (bit < prev_bit) { ok = false; break; }
                }
            }
            if (!ok) continue;

            /* Compute the new state after adding this row. */
            int new_state = 0;
            for (int j = 0; j < NUM_COLS; ++j) {
                int bit = (mask >> j) & 1;
                int d   = state_digits[s][j];
                int nd;
                if (bit == 1)           nd = 1;
                else if (d == 0)        nd = 0;
                else                    nd = 2;    /* d == 1 or 2 */
                new_state += nd * pow3[j];
            }

            /* Merge or add the transition. */
            int idx = -1;
            for (int k = 0; k < trans_count[s]; ++k) {
                if (transitions[s][k].next == new_state) { idx = k; break; }
            }
            if (idx >= 0) {
                transitions[s][idx].cnt += 1;
            } else {
                transitions[s][trans_count[s]].next = new_state;
                transitions[s][trans_count[s]].cnt = 1;
                ++trans_count[s];
            }
        }
    }
}

/* Public sequence function as required. */
seq_t a(idx_t n)
{
    init();

    static seq_t cur[NUM_STATES];
    static seq_t nxt[NUM_STATES];

    /* Reset current state counts. */
    for (int i = 0; i < NUM_STATES; ++i) cur[i] = (seq_t)0;
    cur[0] = (seq_t)1;   /* start with no rows: all columns in state 0 */

    for (idx_t row = 0; row < n; ++row) {
        /* Clear next counts. */
        for (int i = 0; i < NUM_STATES; ++i) nxt[i] = (seq_t)0;

        for (int s = 0; s < NUM_STATES; ++s) {
            seq_t ways = cur[s];
            if (ways == (seq_t)0) continue;
            for (int k = 0; k < trans_count[s]; ++k) {
                const transition_t *tr = &transitions[s][k];
                /* Add contribution to destination state. */
                nxt[tr->next] += ways * (seq_t)tr->cnt;
            }
        }

        /* Copy nxt into cur for the next iteration. */
        for (int i = 0; i < NUM_STATES; ++i) cur[i] = nxt[i];
    }

    /* Sum all ending states. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < NUM_STATES; ++i) total += cur[i];
    return total;
}
