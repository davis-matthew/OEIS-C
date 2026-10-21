/*
 * A207563  Number of nX7 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A207563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Counts n×7 binary arrays that avoid the horizontal patterns
 *  001 and 010, and the vertical patterns 011 and 110.
 *
 *  The algorithm uses dynamic programming on states that
 *  store the last two rows.  There are 31 admissible rows,
 *  so 31×31 = 961 states.  Transitions from (i,j) to (j,k) are
 *  possible iff ((row[i] ^ row[k]) & row[j]) == 0,
 *  which enforces that in every column with a middle bit 1 the
 *  outer bits are equal.  The DP runs in O(n·states) time,
 *  and uses only addition of seq_t values.
 */

#include <stdint.h>
#include <stdbool.h>

/* Fixed limits for the width of the array. */
#define WIDTH          7
#define MAX_ROWS       31          /* number of admissible rows */
#define MAX_STATES     (MAX_ROWS * MAX_ROWS)
#define MAX_TRANS      32          /* maximum transitions per state */

/* Static data that is initialized once. */
static bool initialized = false;

/* Permitted rows (7‑bit patterns). */
static uint8_t row_bits[MAX_ROWS];
static int row_cnt;

/* Transition tables. */
static int next_idx[MAX_STATES][MAX_TRANS];
static int next_cnt[MAX_STATES];

/* DP arrays. */
static seq_t cur_arr[MAX_STATES];
static seq_t next_arr[MAX_STATES];

/* Helper: check whether a 7‑bit pattern contains the banned
 *  horizontal substrings 001 (1) or 010 (2). */
static bool valid_row(uint8_t pat) {
    for (int shift = 0; shift <= WIDTH - 3; ++shift) {
        uint8_t sub = (pat >> shift) & 0x07;   /* 3‑bit window */
        if (sub == 1 || sub == 2) return false; /* 001 or 010 */
    }
    return true;
}

/* One‑time initialization of all tables. */
static void init_once(void) {
    /* --- Enumerate admissible rows --- */
    row_cnt = 0;
    for (uint8_t pat = 0; pat < (1U << WIDTH); ++pat) {
        if (valid_row(pat)) {
            row_bits[row_cnt++] = pat;
        }
    }

    /* --- Build transition tables --- */
    for (int i = 0; i < row_cnt; ++i) {
        for (int j = 0; j < row_cnt; ++j) {
            int state = i * MAX_ROWS + j;
            uint8_t mask = row_bits[j];   /* columns where middle bit is 1 */
            int cnt = 0;
            for (int k = 0; k < row_cnt; ++k) {
                /* Check vertical constraint: ((row[i] ^ row[k]) & mask) == 0 */
                if ( ((row_bits[i] ^ row_bits[k]) & mask) == 0 ) {
                    next_idx[state][cnt++] = j * MAX_ROWS + k;
                }
            }
            next_cnt[state] = cnt;
        }
    }
    initialized = true;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (!initialized) init_once();

    /* Base cases */
    if (n == 1) return (seq_t)31;
    if (n == 2) return (seq_t)961;

    /* DP arrays: pointers to current and next states. */
    seq_t *cur = cur_arr;
    seq_t *next = next_arr;

    /* Initialise cur for sequences of length 2: one for each state. */
    for (int s = 0; s < MAX_STATES; ++s) cur[s] = (seq_t)1;

    /* Iterate from length 3 up to n. */
    for (idx_t len = 3; len <= n; ++len) {
        /* Zero the next array */
        for (int s = 0; s < MAX_STATES; ++s) next[s] = 0;

        /* Perform transitions */
        for (int state = 0; state < MAX_STATES; ++state) {
            seq_t val = cur[state];
            if (val == 0) continue;
            int cnt = next_cnt[state];
            int* list = next_idx[state];
            for (int t = 0; t < cnt; ++t) {
                int nxt = list[t];
                next[nxt] += val;
            }
        }

        /* Swap cur and next for next iteration */
        seq_t *tmp = cur; cur = next; next = tmp;
    }

    /* Sum over final states to get a(n). */
    seq_t total = 0;
    for (int s = 0; s < MAX_STATES; ++s) total += cur[s];

    return total;
}
