/*
 * A229427  Number of nX7 0..2 arrays with horizontal differences mod 3 never
 *          1, vertical differences mod 3 never -1, rows lexicographically
 *          nondecreasing, and columns lexicographically nonincreasing.
 * https://oeis.org/A229427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 16, u64 n <= 96, u128 n <= 210,
 *            big any n
 * Helpers:   lex_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: this implementation computes the sequence A229427 by
 * transfer‑matrix dynamic programming.  The state space consists of
 * (row_pattern , status_bits) where
 *     • row_pattern is one of the 30 admissible rows (non‑increasing
 *       sequences of 7 digits 0..2 that never contain a 2→0 step);
 *     • status_bits (6 bits) describe whether each adjacent column
 *       pair has already been resolved as left>right (bit=1) or is
 *       still equal (bit=0).  A state is valid only if all left<right
 *       possibilities are forbidden.
 *
 * Transitions from a state (i,s) to (j,s') are allowed iff
 *     1. row j lexicographically ≥ row i,
 *     2. vertical constraint: for every column k the difference
 *        (j[k]−i[k]) mod 3 is not 2 (i.e. j[k] may stay or increase
 *        by 1 mod 3),
 *     3. for every position where s has a 0 bit we must have
 *        j[k] ≥ j[k+1]  (otherwise the column would become left<right).
 *
 * The new status bits are s' = s | mask_gt[j], where
 * mask_gt[j] marks positions where j[k] > j[k+1].
 *
 * The algorithm runs in O(n·|states|·avg_degree) time; with the
 * reduced state space (30 patterns × 64 status masks = 1920 states)
 * and very small degree (~10), it easily satisfies the 2‑second
 * limit for n ≤ 10000.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/lex_cmp.h"

/* Precomputed constants ------------------------------------------------- */
#define ROWS 30
#define STATUS 64

/* Global tables for patterns, masks, and adjacency */
static int patterns[ROWS][7];
static unsigned char mask_lt[ROWS];  /* bits where patterns[j][k] < patterns[j][k+1] */
static unsigned char mask_gt[ROWS];  /* bits where patterns[j][k] > patterns[j][k+1] */
static int next_cnt[ROWS];
static int next_idx[ROWS][ROWS];

/* Build all data structures once */
static void init_once(void) {
    /* Step 1: generate all admissible 7‑digit rows (30 in total) */
    int count = 0;
    for (int c2 = 0; c2 <= 7; ++c2) {
        for (int c1 = 0; c1 <= 7 - c2; ++c1) {
            int c0 = 7 - c2 - c1;
            if (c1 == 0 && c2 > 0 && c0 > 0) continue;  /* 2→0 step forbidden */

            int tmp[7];
            int pos = 0;
            for (int t = 0; t < c2; ++t) tmp[pos++]=2;
            for (int t = 0; t < c1; ++t) tmp[pos++]=1;
            for (int t = 0; t < c0; ++t) tmp[pos++]=0;

            for (int k = 0; k < 7; ++k) patterns[count][k] = tmp[k];

            unsigned char lt = 0, gt = 0;
            for (int k = 0; k < 6; ++k) {
                if (tmp[k] < tmp[k+1]) lt |= 1 << k;
                else if (tmp[k] > tmp[k+1]) gt |= 1 << k;
            }
            mask_lt[count] = lt;
            mask_gt[count] = gt;
            ++count;
        }
    }

    /* Step 2: sort the patterns lexicographically (ascending) */
    int ord[ROWS];
    for (int i = 0; i < ROWS; ++i) ord[i] = i;
    /* simple insertion sort */
    for (int i = 1; i < ROWS; ++i) {
        int key = ord[i];
        int j = i - 1;
        while (j >= 0 && lex_cmp(patterns[key], patterns[ord[j]]) < 0) {
            ord[j+1] = ord[j];
            --j;
        }
        ord[j+1] = key;
    }
    /* reorder arrays according to sorted order */
    int pat_tmp[ROWS][7];
    unsigned char lt_tmp[ROWS], gt_tmp[ROWS];
    for (int i = 0; i < ROWS; ++i) {
        for (int k = 0; k < 7; ++k) pat_tmp[i][k] = patterns[ord[i]][k];
        lt_tmp[i] = mask_lt[ord[i]];
        gt_tmp[i] = mask_gt[ord[i]];
    }
    memcpy(patterns, pat_tmp, sizeof(patterns));
    memcpy(mask_lt, lt_tmp, sizeof(mask_lt));
    memcpy(mask_gt, gt_tmp, sizeof(mask_gt));

    /* Step 3: build adjacency lists (i → list of j that satisfy j≥i
       and vertical constraint) */
    for (int i = 0; i < ROWS; ++i) {
        int cnt = 0;
        for (int j = i; j < ROWS; ++j) {   /* j ≥ i */
            bool ok = true;
            for (int k = 0; k < 7; ++k) {
                int diff = patterns[j][k] - patterns[i][k];
                if (diff < 0) diff += 3;          /* mod 3 */
                if (diff == 2) { ok = false; break; }
            }
            if (ok) {
                next_idx[i][cnt++] = j;
            }
        }
        next_cnt[i] = cnt;
    }
}

/* The main sequence function --------------------------------------------- */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_once();
        initialized = true;
    }

    /* DP tables: dp[pattern][status] */
    static seq_t dp[ROWS][STATUS];
    static seq_t dp_next[ROWS][STATUS];

    /* clear dp */
    memset(dp, 0, sizeof(dp));

    /* initial row: any of the 30 patterns with status 0 */
    for (int i = 0; i < ROWS; ++i) dp[i][0] = (seq_t)1;

    /* iterate rows 2..n */
    for (idx_t step = 1; step < n; ++step) {
        /* clear dp_next */
        memset(dp_next, 0, sizeof(dp_next));

        for (int i = 0; i < ROWS; ++i) {
            for (int s = 0; s < STATUS; ++s) {
                seq_t cur = dp[i][s];
                if (cur == 0) continue;
                int cnt = next_cnt[i];
                for (int idx = 0; idx < cnt; ++idx) {
                    int j = next_idx[i][idx];
                    if ((s & mask_lt[j]) != 0) continue;           /* would create left<right */
                    int ns = s | mask_gt[j];
                    dp_next[j][ns] += cur;
                }
            }
        }
        /* swap dp and dp_next */
        memcpy(dp, dp_next, sizeof(dp));
    }

    /* sum all states to obtain a(n) */
    seq_t res = (seq_t)0;
    for (int i = 0; i < ROWS; ++i) {
        for (int s = 0; s < STATUS; ++s) {
            res += dp[i][s];
        }
    }
    return res;
}
