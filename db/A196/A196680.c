/*
 * A196680  Number of nX3 0..4 arrays with each element x equal to the number
 *          its horizontal and vertical neighbors equal to 4,0,1,2,3 for
 *          x=0,1,2,3,4.
 * https://oeis.org/A196680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by dynamic programming over 3‑column patterns
 *            subject to the neighbour–count constraints described in the OEIS
 *            entry for A196680
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

#define COLS 3
#define VALUES 5
#define PATTS 125           /* 5^3 possible rows */
#define STATE_BITS 3        /* 3 variable bits to encode the
                               need for the cell below */
#define STATES (PATTS << STATE_BITS)
#define MAX_TRANS (PATTS)   /* worst case transitions from one state */

/* --- pre‑computed data ------------------------------------------- */

static int pattern[PATTS][COLS];
static int horiz[PATTS][COLS];
static int target[PATTS][COLS];
static int next_state[STATES][MAX_TRANS];
static int next_count[STATES];
static int initialized = 0;

/* Initialize the tables on first call. */
static void init_tables(void)
{
    if (initialized) return;
    initialized = 1;

    /* enumerate all 3‑column rows (base‑5 numbers) */
    for (int pid = 0; pid < PATTS; ++pid) {
        int tmp = pid;
        for (int c = 0; c < COLS; ++c) {
            pattern[pid][c] = tmp % VALUES;
            tmp /= VALUES;
            int v = pattern[pid][c];
            target[pid][c] = (v == 0) ? 4 : v - 1;
        }
    }

    /* horizontal neighbour matches for each row pattern */
    for (int pid = 0; pid < PATTS; ++pid) {
        for (int c = 0; c < COLS; ++c) {
            int t = target[pid][c];
            int h = 0;
            if (c > 0   && pattern[pid][c - 1] == t) ++h;
            if (c < 2   && pattern[pid][c + 1] == t) ++h;
            horiz[pid][c] = h;
        }
    }

    /* compute transitions between states */
    for (int s = 0; s < STATES; ++s) {
        int prev_id   = s >> STATE_BITS;
        int prev_bits = s & ((1 << STATE_BITS) - 1);
        int cnt = 0;

        for (int cur_id = 0; cur_id < PATTS; ++cur_id) {
            int ok = 1;
            int next_bits = 0;

            for (int c = 0; c < COLS; ++c) {
                int val_cur  = pattern[cur_id][c];
                int t_cur    = target[cur_id][c];
                int val_prev = pattern[prev_id][c];
                int t_prev   = target[prev_id][c];
                int req_prev = (prev_bits >> c) & 1;

                /* must satisfy previous cell's requirement for below */
                if (req_prev) {
                    if (val_cur != t_prev) { ok = 0; break; }
                } else {
                    if (val_cur == t_prev) { ok = 0; break; }
                }

                /* check vertical match with above cell */
                int eq_current = (val_prev == t_cur) ? 1 : 0;
                int need_cur   = val_cur - (horiz[cur_id][c] + eq_current);
                if (need_cur < 0 || need_cur > 1) { ok = 0; break; }

                next_bits |= (need_cur << c);
            }

            if (ok) {
                next_state[s][cnt++] = cur_id * (1 << STATE_BITS) + next_bits;
            }
        }
        next_count[s] = cnt;
    }
}

/* --------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    init_tables();

    /* state counts for current and next rows */
    seq_t cur[STATES];
    seq_t nxt[STATES];

    /* zero the arrays */
    for (int i = 0; i < STATES; ++i) cur[i] = 0;

    /* --- base case: first row (no above neighbours) ----------- */
    for (int pid = 0; pid < PATTS; ++pid) {
        int ok = 1;
        int init_bits = 0;

        for (int c = 0; c < COLS; ++c) {
            int val  = pattern[pid][c];
            int need = val - horiz[pid][c];
            if (need < 0 || need > 1) { ok = 0; break; }
            init_bits |= (need << c);
        }

        if (ok) {
            int idx = pid * (1 << STATE_BITS) + init_bits;
            cur[idx] += (seq_t)1;
        }
    }

    /* if only one row, sum states with no pending matches */
    if (n == 1) {
        seq_t ans = 0;
        for (int pid = 0; pid < PATTS; ++pid) {
            ans += cur[pid * (1 << STATE_BITS) + 0];
        }
        return ans;
    }

    /* --- DP over remaining rows -------------------------------- */
    for (idx_t row = 2; row <= n; ++row) {
        /* zero next array */
        for (int i = 0; i < STATES; ++i) nxt[i] = 0;

        for (int s = 0; s < STATES; ++s) {
            if (cur[s] == 0) continue;
            seq_t val = cur[s];
            int cnt = next_count[s];

            for (int t_index = 0; t_index < cnt; ++t_index) {
                int t = next_state[s][t_index];
                nxt[t] += val;
            }
        }
        /* copy nxt → cur */
        for (int i = 0; i < STATES; ++i) cur[i] = nxt[i];
    }

    /* sum final states with no pending matches */
    seq_t ans = 0;
    for (int pid = 0; pid < PATTS; ++pid) {
        ans += cur[pid * (1 << STATE_BITS) + 0];
    }
    return ans;
}
