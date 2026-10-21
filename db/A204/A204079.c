/*
 * A204079  Number of nX3 0..3 arrays with every element neighboring
 *          horizontally or vertically both a 0 and a 1.
 * https://oeis.org/A204079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   decode_pattern
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n):  n×3 arrays over {0,1,2,3} where every element has
 * at least one horizontally or vertically adjacent element equal to 0
 * and at least one such adjacent element equal to 1.
 *
 * Computed by dynamic programming over rows.
 * A state is defined by (row_pattern, pending_need_mask), where
 * row_pattern is a pattern of length 3 (4^3 = 64 possibilities)
 * and pending_need_mask is a 2‑bit mask per column
 *   (bit 0 of column j set → 0 still needed,
 *    bit 1 of column j set → 1 still needed).
 * Transitions check that the new row satisfies the pending needs
 * of the previous row and compute the new pending needs for the
 * current row.  After processing the last row we only accept
 * states with no pending needs. */

#include <stddef.h>
#include "../utils/decode_pattern.h"

/* ------------------------------------------------------------------ */
/*  State tables and transition calculations                          */
/* ------------------------------------------------------------------ */
#define COLS      3
#define PAT_CNT   64          /* 4^3 */
#define NEED_CNT  27          /* 3^3 possibilities for 2‑bit masks with no both bits set */

/* Number of states */
#define STATE_CNT  (PAT_CNT * NEED_CNT)

/* decode tables */
static unsigned char pat_col[PAT_CNT][COLS];
static int         mask_to_idx[64];      /* 6 bits → need index, -1 if invalid */
static int         need_mask[NEED_CNT];  /* index → 6‑bit mask */

/* index of empty need mask */
static int idx_zero_mask;

/* transition table: for each state, list of next states */
static unsigned short trans_states[STATE_CNT][PAT_CNT];
static unsigned char   trans_count[STATE_CNT];

static int transitions_inited = 0;

/* ------------------------------------------------------------------ */
/*  Transition table initialisation                                   */
/* ------------------------------------------------------------------ */
static void init_transitions(void) {
    unsigned i, j, k, t;
    unsigned mask, idx;
    unsigned char colP[COLS], colPP[COLS];
    int ok;

    /* initialise mask lookup: 6‑bit mask → need index */
    for (i = 0; i < 64; ++i) mask_to_idx[i] = -1;

    /* build list of valid need masks (no column requires both 0 and 1) */
    int unknowns = 0;
    for (mask = 0; mask < 64; ++mask) {
        ok = 1;
        for (j = 0; j < COLS; ++j) {
            int m = (mask >> (2 * j)) & 3;
            if (m == 3) { ok = 0; break; }      /* column needs both 0 and 1 → impossible */
        }
        if (ok) {
            mask_to_idx[mask] = unknowns;
            need_mask[unknowns] = (int)mask;
            if (mask == 0) idx_zero_mask = unknowns;
            ++unknowns;
        }
    }
    /* unknowns should equal NEED_CNT */
    (void)unknowns;  /* silence unused variable warning */

    /* precompute decoded patterns */
    for (i = 0; i < PAT_CNT; ++i) decode_pattern((unsigned)i, pat_col[i]);

    /* build transitions */
    for (i = 0; i < PAT_CNT; ++i) {           /* current pattern */
        unsigned char *colP = pat_col[i];
        for (k = 0; k < NEED_CNT; ++k) {    /* current pending need mask index */
            int maskN = need_mask[k];
            size_t s = i * NEED_CNT + k;
            trans_count[s] = 0;

            for (t = 0; t < PAT_CNT; ++t) {    /* candidate next pattern */
                unsigned char *colPP = pat_col[t];
                int good = 1;

                /* 1. check that next row satisfies all pending needs of current row */
                for (j = 0; j < COLS; ++j) {
                    int need0 = (maskN >> (2 * j)) & 1;
                    int need1 = (maskN >> (2 * j + 1)) & 1;
                    int c = colPP[j];
                    if (need0 && c != 0) { good = 0; break; }
                    if (need1 && c != 1) { good = 0; break; }
                }
                if (!good) continue;

                /* 2. compute new pending need mask for the next row */
                mask = 0;
                int bad = 0;
                for (j = 0; j < COLS; ++j) {
                    int has0 = 0, has1 = 0;

                    /* up neighbour (current row) */
                    switch (colP[j]) {
                        case 0: has0 = 1; break;
                        case 1: has1 = 1; break;
                    }

                    /* left neighbour (next row) */
                    if (j > 0) {
                        switch (colPP[j - 1]) {
                            case 0: has0 = 1; break;
                            case 1: has1 = 1; break;
                        }
                    }

                    /* right neighbour (next row) */
                    if (j < COLS - 1) {
                        switch (colPP[j + 1]) {
                            case 0: has0 = 1; break;
                            case 1: has1 = 1; break;
                        }
                    }

                    /* If neither 0 nor 1 found yet, we would need both from the
                     * down neighbour – impossible. */
                    if (!has0 && !has1) { bad = 1; break; }

                    if (!has0 && has1) mask |= (1 << (2 * j));   /* need 0 */
                    if (has0 && !has1) mask |= (2 << (2 * j));   /* need 1 */
                }
                if (bad) continue;

                idx = mask_to_idx[mask];
                if (idx < 0) continue;  /* should not happen */

                trans_states[s][trans_count[s]++] = (unsigned short)(t * NEED_CNT + idx);
            }
        }
    }
    transitions_inited = 1;
}

/* ------------------------------------------------------------------ */
/*  Sequence function                                                 */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined – safe fallback */

    /* initialise tables on first call */
    if (!transitions_inited) init_transitions();

    /* DP arrays */
    seq_t cur[STATE_CNT];
    seq_t nxt[STATE_CNT];

    idx_t i, r;
    for (i = 0; i < STATE_CNT; ++i) cur[i] = (seq_t)0;

    /* ---- initialise states after processing the first row ---- */
    unsigned char mask, col[COLS];
    for (i = 0; i < PAT_CNT; ++i) {
        unsigned char *colP = pat_col[(unsigned)i];
        mask = 0;
        int bad = 0;
        for (int j = 0; j < COLS; ++j) {
            int has0 = 0, has1 = 0;
            /* left neighbour of first row */
            if (j > 0) {
                switch (colP[j - 1]) {
                    case 0: has0 = 1; break;
                    case 1: has1 = 1; break;
                }
            }
            /* right neighbour of first row */
            if (j < COLS - 1) {
                switch (colP[j + 1]) {
                    case 0: has0 = 1; break;
                    case 1: has1 = 1; break;
                }
            }
            /* no up neighbour */
            if (!has0 && !has1) { bad = 1; break; }  /* would need both from below */
            if (!has0 && has1) mask |= (1 << (2 * j));
            if (has0 && !has1) mask |= (2 << (2 * j));
        }
        if (bad) continue;
        int idx = mask_to_idx[mask];
        if (idx < 0) continue;
        cur[(unsigned)i * NEED_CNT + idx] += (seq_t)1;
    }

    /* ---- process remaining rows ---- */
    for (r = 2; r <= (size_t)n; ++r) {
        for (i = 0; i < STATE_CNT; ++i) nxt[i] = (seq_t)0;

        for (i = 0; i < STATE_CNT; ++i) {
            if (cur[i] == (seq_t)0) continue;
            unsigned char cnt = trans_count[i];
            for (unsigned char t = 0; t < cnt; ++t) {
                nxt[ trans_states[i][t] ] += cur[i];
            }
        }
        for (i = 0; i < STATE_CNT; ++i) cur[i] = nxt[i];
    }

    /* ---- sum states that have no pending needs (mask 0) ---- */
    seq_t ans = (seq_t)0;
    for (i = 0; i < PAT_CNT; ++i) {
        ans += cur[(unsigned)i * NEED_CNT + idx_zero_mask];
    }
    return ans;
}
