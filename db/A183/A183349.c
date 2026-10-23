/*
 * A183349  Number of nX7 binary arrays with each 1 adjacent to exactly one 0
 *          vertically and one 0 horizontally.
 * https://oeis.org/A183349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of n×7 binary arrays in which every 1 has
 * exactly one adjacent 0 vertically and exactly one adjacent 0 horizontally.
 *
 * This implementation uses a transfer‑matrix dynamic programming.  
 * Each state is a pair of consecutive rows (mask values).  The
 * vertical constraint for the middle row is checked when a new row
 * is appended, and the horizontal constraint is enforced for each
 * row individually.  The algorithm runs in O(n·S·G) with
 * S = number of good row masks (≤128) and G = number of admissible
 * continuations per state (≈20), easily fitting in the required
 * time limits.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* constant parameters */
#define W            7                    /* width of the array */
#define MASK_S       (1 << W)              /* possible row masks = 128 */
#define MAX_STATES   (MASK_S * MASK_S)     /* maximum number of states  = 16384 */

/* data structures used by the routine */
static bool init_done = false;

static uint8_t  good_mask_list[MASK_S];   /* list of masks satisfying horizontal rule */
static unsigned int  good_cnt = 0;        /* number of such masks */
static int     mask_to_id[MASK_S];       /* map mask → id (-1 if not good) */

static unsigned char  succ_cnt[MAX_STATES];          /* number of successors of a state */
static uint8_t        succ_list[MAX_STATES][MASK_S];/* list of successor ids */

/* DP arrays (two copies to avoid copying whole array inside the loop) */
static seq_t dp_cur[MAX_STATES];
static seq_t dp_next[MAX_STATES];

/* ------------------------------------------------------------------ */
/* Helper: test whether a mask satisfies the horizontal rule.        */
static bool is_good_row(uint8_t mask)
{
    for (int j = 0; j < W; ++j) {
        if (mask & (1 << j)) { /* this position is a 1 */
            int zeros = 0;
            if (j > 0)  zeros += ((mask >> (j - 1)) & 1) == 0;
            if (j < W - 1) zeros += ((mask >> (j + 1)) & 1) == 0;
            if (zeros != 1)  /* not exactly one zero neighbor */
                return false;
        }
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Pre‑compute good masks and transition table. */
static void init_tables(void)
{
    if (init_done) return;
    init_done = true;

    /* enumerate good row masks and build mask→id table */
    for (int m = 0; m < MASK_S; ++m) {
        mask_to_id[m] = -1;
    }
    for (int m = 0; m < MASK_S; ++m) {
        if (is_good_row((uint8_t)m)) {
            mask_to_id[m] = (int)good_cnt;
            good_mask_list[good_cnt++] = (uint8_t)m;
        }
    }

    /* build adjacency (successor) lists for all state pairs */
    for (int u_id = 0; u_id < good_cnt; ++u_id) {
        for (int v_id = 0; v_id < good_cnt; ++v_id) {
            int idx = u_id * MASK_S + v_id;
            unsigned cnt = 0;
            uint8_t mask_u = good_mask_list[u_id];
            uint8_t mask_v = good_mask_list[v_id];
            for (int c_id = 0; c_id < good_cnt; ++c_id) {
                uint8_t mask_c = good_mask_list[c_id];
                /* vertical rule for mask_v (the middle row):   (u ^ c) & v == v */
                if (((mask_u ^ mask_c) & mask_v) == mask_v) {
                    succ_list[idx][cnt++] = (uint8_t)c_id;
                }
            }
            succ_cnt[idx] = (unsigned char)cnt;
        }
    }
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;   /* only the all‑zero array */

    init_tables();

    /* initialise dp for the first two rows: only rows with no
       overlapping 1s are allowed (top row has no upper neighbour, the
       bottom row has no lower neighbour). */
    memset(dp_cur, 0, sizeof(dp_cur));

    for (int i = 0; i < good_cnt; ++i) {
        uint8_t mask_i = good_mask_list[i];
        for (int j = 0; j < good_cnt; ++j) {
            uint8_t mask_j = good_mask_list[j];
            if ((mask_i & mask_j) == 0) {
                int idx = i * MASK_S + j;   /* state (up=i, prev=j) */
                dp_cur[idx] += (seq_t)1;
            }
        }
    }

    if (n == 2) {
        /* sum over all valid state pairs; they all satisfy the bottom row rule. */
        seq_t result = 0;
        for (int i = 0; i < good_cnt; ++i) {
            for (int j = 0; j < good_cnt; ++j) {
                if ((good_mask_list[i] & good_mask_list[j]) == 0) {
                    int idx = i * MASK_S + j;
                    result += dp_cur[idx];
                }
            }
        }
        return result;
    }

    /* DP for rows 3 … n */
    for (idx_t row = 3; row <= n; ++row) {
        memset(dp_next, 0, sizeof(dp_next));

        for (int u_id = 0; u_id < good_cnt; ++u_id) {
            for (int v_id = 0; v_id < good_cnt; ++v_id) {
                int idx = u_id * MASK_S + v_id;
                seq_t ways = dp_cur[idx];
                if (ways == 0) continue;

                unsigned cnt = succ_cnt[idx];
                for (unsigned k = 0; k < cnt; ++k) {
                    int next_id = succ_list[idx][k];
                    int new_idx = v_id * MASK_S + next_id;  /* new state (prev=v_id, cur=next_id) */
                    dp_next[new_idx] += ways;
                }
            }
        }
        /* swap dp arrays for next iteration */
        memcpy(dp_cur, dp_next, sizeof(dp_cur));
    }

    /* final condition : the last row must have no vertical 1‑overlap
       with the previous row (i.e., (up & prev) == 0). */
    seq_t result = 0;
    for (int u_id = 0; u_id < good_cnt; ++u_id) {
        uint8_t mask_u = good_mask_list[u_id];
        for (int v_id = 0; v_id < good_cnt; ++v_id) {
            uint8_t mask_v = good_mask_list[v_id];
            if ((mask_u & mask_v) == 0) {
                int idx = u_id * MASK_S + v_id;
                result += dp_cur[idx];
            }
        }
    }
    return result;
}
