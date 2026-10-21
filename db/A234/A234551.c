/*
 * A234551  Number of (n+1) X (2+1) 0..6 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 3
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of (n+1)×3 0..6 arrays with every 2×2 subblock
 *            having its diagonal sum differing from its antidiagonal sum by 3
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   absint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/absint.h"

/* Compile‑time constants */
enum { MAX_DIFF   = 6,
       DIFF_RANGE = 2 * MAX_DIFF + 1,  /* 13 */
       MAX_STATES = DIFF_RANGE * DIFF_RANGE }; /* 169 */

/* State representation */
typedef struct {
    int d1;                          /* x0 - x1   */
    int d2;                          /* x1 - x2   */
    seq_t mult;                      /* number of rows giving this pair of differences */
    size_t next[4];                  /* indices of admissible next states (max 4) */
    size_t next_len;                 /* how many admissible next states */
} State;

static State states[MAX_STATES];
static size_t state_cnt = 0;
static bool initialized = false;

seq_t a(idx_t n)
{
    /* Initialise the state space and adjacency lists on first call. */
    if (!initialized) {
        initialized = true;

        /* Enumerate all feasible difference pairs and their multiplicities. */
        for (int d1 = -MAX_DIFF; d1 <= MAX_DIFF; ++d1) {
            for (int d2 = -MAX_DIFF; d2 <= MAX_DIFF; ++d2) {
                int count = 0;
                for (int x1 = 0; x1 <= 6; ++x1) {
                    int x0 = x1 + d1;
                    int x2 = x1 - d2;
                    if (x0 >= 0 && x0 <= 6 && x2 >= 0 && x2 <= 6)
                        ++count;
                }
                if (count) {
                    size_t s = state_cnt++;
                    states[s].d1       = d1;
                    states[s].d2       = d2;
                    states[s].mult     = (seq_t)count;
                    states[s].next_len = 0;
                }
            }
        }

        /* Build adjacency lists: |d1 - d1'| = 3 and |d2 - d2'| = 3. */
        for (size_t i = 0; i < state_cnt; ++i) {
            for (size_t j = 0; j < state_cnt; ++j) {
                if (absint(states[i].d1 - states[j].d1) == 3 &&
                    absint(states[i].d2 - states[j].d2) == 3) {
                    states[i].next[states[i].next_len++] = j;
                }
            }
        }
    }

    /* Dynamic programming over states.  curr[i] = number of arrays of current
       length ending in state i. */
    seq_t curr[MAX_STATES];
    seq_t next[MAX_STATES];
    for (size_t i = 0; i < state_cnt; ++i)
        curr[i] = states[i].mult;           /* length 1 – any single row */

    /* Perform n transitions to reach length n+1 rows. */
    for (idx_t step = 1; step <= n; ++step) {
        for (size_t i = 0; i < state_cnt; ++i)
            next[i] = 0;

        for (size_t i = 0; i < state_cnt; ++i) {
            seq_t val = curr[i];
            if (!val) continue;
            for (size_t k = 0; k < states[i].next_len; ++k) {
                size_t j = states[i].next[k];
                next[j] += val * states[j].mult;
            }
        }

        for (size_t i = 0; i < state_cnt; ++i)
            curr[i] = next[i];
    }

    /* Sum over all final states to get total number of arrays of size (n+1)×3. */
    seq_t total = 0;
    for (size_t i = 0; i < state_cnt; ++i)
        total += curr[i];

    return total;
}
