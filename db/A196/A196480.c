/*
 * A196480  Number of n X 3 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 3,0,2,1,4
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Non‑recursive DP: counts n×3 arrays satisfying the neighbour‑count constraints. */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

static const int TARGET[5] = {3, 0, 2, 1, 4};
static const int REQ[5]    = {0, 1, 2, 3, 4};

#define ROWS 125          /* 5^3 possible rows */
#define STATES (ROWS*64)  /* pending counts encoded in 3*2 bits */

/* Precomputed data for each row code (0..124) */
static uint8_t row_val[ROWS][3];          /* values in the row */
static uint8_t horiz_match[ROWS][3];      /* horizontal matches for each cell */

/* Transition data: transition_start[s] .. transition_start[s+1]-1 give indices in
   transition_dest[tot_trans] where destinations are valid subsequent states. */
static size_t transition_start[STATES+1];
static uint16_t *transition_dest = NULL;

/* DP arrays */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;

/* Helper to convert an integer code to its base‑5 digits (a,b,c). */
static void decode_row(int code, uint8_t* a, uint8_t* b, uint8_t* c) {
    *a = (unsigned)(code / 25);
    *b = (unsigned)((code / 5) % 5);
    *c = (unsigned)(code % 5);
}

/* Precompute all tables once. */
static void init_tables(void) {
    /* Row values and horizontal matches */
    for (int code = 0; code < ROWS; ++code) {
        uint8_t a,b,c;
        decode_row(code, &a, &b, &c);
        row_val[code][0] = a;
        row_val[code][1] = b;
        row_val[code][2] = c;
        for (int i = 0; i < 3; ++i) {
            uint8_t tgt = TARGET[row_val[code][i]];
            uint8_t cnt = 0;
            if (i > 0 && row_val[code][i-1] == tgt) ++cnt;
            if (i < 2 && row_val[code][i+1] == tgt) ++cnt;
            horiz_match[code][i] = cnt;
        }
    }

    /* First pass: count transitions */
    size_t tot = 0;
    for (int s = 0; s < STATES; ++s) {
        int prev_code = s / 64;
        uint8_t prev_mask = s % 64;
        uint8_t prev_pending[3];
        prev_pending[0] =  (prev_mask      ) & 3;
        prev_pending[1] = (prev_mask >> 2) & 3;
        prev_pending[2] = (prev_mask >> 4) & 3;
        int trans = 0;
        for (int curr_code = 0; curr_code < ROWS; ++curr_code) {
            /* Check if transition is valid */
            int ok = 1;
            uint8_t curr_mask = 0;
            for (int i = 0; i < 3; ++i) {
                /* Contribution to previous cell from this row */
                int cont_prev = (row_val[curr_code][i] == TARGET[row_val[prev_code][i]]) ? 1 : 0;
                int total = prev_pending[i] + cont_prev;
                if (total != REQ[row_val[prev_code][i]]) {
                    ok = 0; break;
                }
                /* Pending count for current cell (up + horizontal) */
                int up_contrib = (row_val[prev_code][i] == TARGET[row_val[curr_code][i]]) ? 1 : 0;
                int cur_pending = up_contrib + horiz_match[curr_code][i];
                curr_mask |= (cur_pending & 3) << (2*i);
            }
            if (ok) ++trans;
        }
        transition_start[s] = tot;
        tot += trans;
    }
    transition_start[STATES] = tot;

    /* Allocate array for destination states */
    transition_dest = (uint16_t*)malloc(tot * sizeof(uint16_t));

    /* Second pass: fill destinations */
    tot = 0;
    for (int s = 0; s < STATES; ++s) {
        int prev_code = s / 64;
        uint8_t prev_mask = s % 64;
        uint8_t prev_pending[3];
        prev_pending[0] =  (prev_mask      ) & 3;
        prev_pending[1] = (prev_mask >> 2) & 3;
        prev_pending[2] = (prev_mask >> 4) & 3;
        for (int curr_code = 0; curr_code < ROWS; ++curr_code) {
            int ok = 1;
            uint8_t curr_mask = 0;
            for (int i = 0; i < 3; ++i) {
                int cont_prev = (row_val[curr_code][i] == TARGET[row_val[prev_code][i]]) ? 1 : 0;
                int total = prev_pending[i] + cont_prev;
                if (total != REQ[row_val[prev_code][i]]) {
                    ok = 0; break;
                }
                int up_contrib = (row_val[prev_code][i] == TARGET[row_val[curr_code][i]]) ? 1 : 0;
                int cur_pending = up_contrib + horiz_match[curr_code][i];
                curr_mask |= (cur_pending & 3) << (2*i);
            }
            if (ok) {
                uint16_t dest = curr_code*64 + curr_mask;
                transition_dest[tot++] = dest;
            }
        }
    }

    /* Allocate DP arrays */
    dp_cur  = (seq_t*)malloc(STATES * sizeof(seq_t));
    dp_next = (seq_t*)malloc(STATES * sizeof(seq_t));
}

seq_t a(idx_t n) {
    /* Initialize tables on first call */
    static int initialized = 0;
    if (!initialized) {
        init_tables();
        initialized = 1;
    }

    if (n == 0) return (seq_t)0; /* undefined; return 0 */

    size_t i;
    /* Initial DP for the first row */
    memset(dp_cur, 0, STATES * sizeof(seq_t));
    for (int code = 0; code < ROWS; ++code) {
        uint8_t mask = 0;
        for (int j = 0; j < 3; ++j)
            mask |= (horiz_match[code][j] & 3) << (2*j);
        dp_cur[code*64 + mask] = (seq_t)1;
    }

    /* For n == 1 we skip transitions */
    if (n > 1) {
        for (idx_t row = 1; row < n; ++row) {
            memset(dp_next, 0, STATES * sizeof(seq_t));
            for (i = 0; i < STATES; ++i) {
                seq_t cnt = dp_cur[i];
                if (!cnt) continue;
                size_t start = transition_start[i];
                size_t end   = transition_start[i+1];
                for (size_t t = start; t < end; ++t) {
                    uint16_t dest = transition_dest[t];
                    dp_next[dest] += cnt;
                }
            }
            /* swap dp arrays */
            seq_t *tmp = dp_cur; dp_cur = dp_next; dp_next = tmp;
        }
    }

    /* Sum valid final states */
    seq_t ans = (seq_t)0;
    for (i = 0; i < STATES; ++i) {
        seq_t cnt = dp_cur[i];
        if (!cnt) continue;
        int code = i / 64;
        uint8_t mask = i % 64;
        uint8_t pending[3];
        pending[0] =  (mask      ) & 3;
        pending[1] = (mask >> 2) & 3;
        pending[2] = (mask >> 4) & 3;
        int ok = 1;
        for (int j = 0; j < 3; ++j) {
            if (pending[j] != REQ[row_val[code][j]]) { ok = 0; break; }
        }
        if (ok) ans += cnt;
    }
    return ans;
}
