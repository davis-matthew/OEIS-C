/*
 * A196134  Number of n X 4 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 0,2,1,3,4
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* DP implementation for OEIS A196134: count of n×4 arrays with the
   neighbour‑count constraints described in the entry.  The algorithm uses
   a transfer‑matrix over row patterns;  the state consists of the
   pattern of the current row and a 4‑bit mask telling for each cell whether
   it still needs one matching neighbour below.  Transitions are pre‑
   computed once, then the DP is performed for the requested n.  */
/* It works for any integer type seq_t and idx_t as defined by the build. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define PATTERN_COUNT 625   /* 5^4 */
#define MASK_BITS     4
#define MASK_COUNT    (1 << MASK_BITS)
#define STATE_COUNT   (PATTERN_COUNT * MASK_COUNT)

static const int target_of[5] = {0, 2, 1, 3, 4};   /* mapping described */
static const int req_of[5]    = {0, 1, 2, 3, 4};

static int pattern_col[PATTERN_COUNT][4];
static int target_col[PATTERN_COUNT][4];
static int horiz_match[PATTERN_COUNT][4];
static int pow5[5] = {1, 5, 25, 125, 625};

static int initialized = 0;
static int transitions[STATE_COUNT][256];   /* 256 is an upper bound */
static unsigned char trans_cnt[STATE_COUNT];

/* ----------------------------------------------------------------------- */
static void init_tables(void)
{
    if (initialized) return;
    initialized = 1;

    /* decompose each pattern into its four digits (columns) */
    for (int p = 0; p < PATTERN_COUNT; ++p) {
        int tmp = p;
        for (int i = 3; i >= 0; --i) {
            pattern_col[p][i] = tmp % 5;
            tmp /= 5;
        }
        for (int i = 0; i < 4; ++i) {
            target_col[p][i] = target_of[ pattern_col[p][i] ];
        }
        for (int i = 0; i < 4; ++i) {
            int cnt = 0;
            if (i > 0 && pattern_col[p][i-1] == target_col[p][i]) ++cnt;
            if (i < 3 && pattern_col[p][i+1] == target_col[p][i]) ++cnt;
            horiz_match[p][i] = cnt;
        }
    }

    /* pre‑compute all transitions */
    for (int p = 0; p < PATTERN_COUNT; ++p) {
        for (int mask = 0; mask < MASK_COUNT; ++mask) {
            int state_idx = p * MASK_COUNT + mask;
            unsigned char cnt = 0;
            for (int q = 0; q < PATTERN_COUNT; ++q) {
                /* first, check that q satisfies the residual requirement of p */
                int ok = 1;
                for (int i = 0; i < 4; ++i) {
                    int b = (q == 0) ? 0 : 0; /* placeholder to silence warning */
                    int match = (pattern_col[q][i] == target_col[p][i]) ? 1 : 0;
                    if (match != ((mask >> i) & 1)) { ok = 0; break; }
                }
                if (!ok) continue;

                /* compute new partial counts for q and build new mask */
                int new_mask = 0;
                for (int i = 0; i < 4; ++i) {
                    int up_match = (pattern_col[p][i] == target_col[q][i]) ? 1 : 0;
                    int part = horiz_match[q][i] + up_match;
                    if (part > req_of[ pattern_col[q][i] ]) { ok = 0; break; }
                    int rem = req_of[ pattern_col[q][i] ] - part;
                    if (rem == 1) new_mask |= (1 << i);
                    else if (rem == 0) continue;
                    else { ok = 0; break; }   /* rem > 1 not allowed */
                }
                if (!ok) continue;

                transitions[state_idx][cnt++] = q * MASK_COUNT + new_mask;
            }
            trans_cnt[state_idx] = cnt;
        }
    }
}

/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* outside the defined offset, return 0 */

    init_tables();

    static seq_t *dp_cur = NULL;
    static seq_t *dp_next = NULL;
    static size_t dp_size = 0;

    if (dp_size < STATE_COUNT) {
        free(dp_cur); free(dp_next);
        dp_cur   = (seq_t*)malloc(STATE_COUNT * sizeof(seq_t));
        dp_next  = (seq_t*)malloc(STATE_COUNT * sizeof(seq_t));
        dp_size = STATE_COUNT;
    }
    for (size_t i = 0; i < STATE_COUNT; ++i) {
        dp_cur[i]  = 0;
        dp_next[i] = 0;
    }

    /* ----- first row --------------------------------------------------- */
    for (int p = 0; p < PATTERN_COUNT; ++p) {
        int mask = 0;
        int ok = 1;
        for (int i = 0; i < 4; ++i) {
            int part = horiz_match[p][i];   /* left/right only, no up neighbor */
            int rem  = req_of[ pattern_col[p][i] ] - part;
            if (rem < 0 || rem > 1) { ok = 0; break; }
            if (rem == 1) mask |= (1 << i);
        }
        if (!ok) continue;
        int s = p * MASK_COUNT + mask;
        dp_cur[s] += 1;
    }

    /* ----- process remaining rows -------------------------------------- */
    for (idx_t row = 1; row < n; ++row) {
        /* zero next array */
        for (size_t i = 0; i < STATE_COUNT; ++i) dp_next[i] = 0;

        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp_cur[s];
            if (val == 0) continue;
            unsigned char cnt = trans_cnt[s];
            int *tlist = transitions[s];
            for (unsigned char k = 0; k < cnt; ++k) {
                int ns = tlist[k];
                dp_next[ns] += val;
            }
        }
        /* swap */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* ----- last row: only states with mask=0 are valid ----------------- */
    seq_t result = 0;
    for (int p = 0; p < PATTERN_COUNT; ++p) {
        int s = p * MASK_COUNT;  /* mask == 0 */
        result += dp_cur[s];
    }
    return result;
}
