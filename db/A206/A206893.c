/*
 * A206893  Number of 7Xn 0..1 arrays avoiding 0 1 0 horizontally and 1 0 0
 *          vertically.
 * https://oeis.org/A206893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Code computes a(n) = number of 7×n binary arrays avoiding 0‑1‑0 horizontally
   and 1‑0‑0 vertically.  It uses a transfer–matrix dynamic program over
   the 54 admissible column patterns. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* number of admissible column patterns (no vertical 1‑0‑0) */
#define COLS 54

/* transition count and list: transCount[i][j] gives number of allowed next
   columns after column pair (i,j); transIndex[i][j][k] gives those columns. */
static uint8_t transCount[COLS][COLS];
static uint8_t transIndex[COLS][COLS][COLS];

/* Memoisation cache for a(n).  memo[0] is unused. */
static seq_t *memo = NULL;
static size_t memo_cap = 0;

/* pre‑computed valid column masks */
static uint8_t columnMask[COLS];

/* DP tables for current and previous column pairs */
static seq_t dpPrev[COLS][COLS];
static seq_t dpCurr[COLS][COLS];

/* one‑time initialisation flag */
static bool init_done = false;

/* Build the list of admissible column masks (no 1‑0‑0 vertically). */
static void build_columns(void)
{
    uint8_t cnt = 0;
    for (uint8_t mask = 0; mask < (1U << 7); ++mask) {
        bool ok = true;
        for (int r = 0; r <= 4; ++r) {
            int b0 = (mask >> r)     & 1U;
            int b1 = (mask >> (r+1)) & 1U;
            int b2 = (mask >> (r+2)) & 1U;
            if (b0 == 1 && b1 == 0 && b2 == 0) { ok = false; break; }
        }
        if (ok) columnMask[cnt++] = mask;
    }
}

/* Build transition tables: for each pair of columns (i,j) compute all
   columns k that together with i and j avoid the forbidden horizontal 0‑1‑0. */
static void build_transitions(void)
{
    for (int i = 0; i < COLS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            /* rows where previous=0 and current=1 force next=1 */
            uint8_t force = 0;
            for (int r = 0; r < 7; ++r) {
                int a = (columnMask[i] >> r) & 1;
                int b = (columnMask[j] >> r) & 1;
                if (a == 0 && b == 1) force |= 1U << r;
            }
            uint8_t nb = 0;
            for (int k = 0; k < COLS; ++k) {
                if ((columnMask[k] & force) == force) {
                    transIndex[i][j][nb++] = (uint8_t)k;
                }
            }
            transCount[i][j] = nb;
        }
    }
}

/* Ensure that the memoisation array can store up to index n. */
static void ensure_memo(idx_t n)
{
    if (memo == NULL) {
        memo_cap = 128;
        memo = (seq_t*)malloc(sizeof(seq_t) * memo_cap);
        for (size_t i = 0; i < memo_cap; ++i) memo[i] = 0;
    }
    if ((size_t)n >= memo_cap) {
        size_t new_cap = memo_cap;
        while ((size_t)n >= new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(memo, sizeof(seq_t) * new_cap);
        if (!tmp) return; /* allocation failure – caller will handle */
        for (size_t i = memo_cap; i < new_cap; ++i) tmp[i] = 0;
        memo = tmp;
        memo_cap = new_cap;
    }
}

/* The sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset not used */
    ensure_memo(n);
    if (memo[n] != 0) return memo[n];

    /* one‑time set‑up of columns and transitions */
    if (!init_done) {
        build_columns();
        build_transitions();
        init_done = true;
    }

    if (n == 1) {
        memo[n] = (seq_t)COLS;
        return memo[n];
    }
    if (n == 2) {
        memo[n] = (seq_t)COLS * (seq_t)COLS;
        return memo[n];
    }

    /* initialise dpPrev for n = 2: every ordered pair of columns is possible */
    for (int i = 0; i < COLS; ++i)
        for (int j = 0; j < COLS; ++j)
            dpPrev[i][j] = 1;

    /* dynamic programming for columns 3..n */
    for (idx_t col = 3; col <= n; ++col) {
        /* zero dpCurr */
        for (int i = 0; i < COLS; ++i)
            for (int j = 0; j < COLS; ++j)
                dpCurr[i][j] = 0;

        /* transition from (i,j) to (j,k) */
        for (int i = 0; i < COLS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                seq_t val = dpPrev[i][j];
                if (val == 0) continue;
                uint8_t nb = transCount[i][j];
                for (uint8_t t = 0; t < nb; ++t) {
                    int k = transIndex[i][j][t];
                    dpCurr[j][k] += val;
                }
            }
        }
        /* swap dpPrev and dpCurr */
        for (int i = 0; i < COLS; ++i)
            for (int j = 0; j < COLS; ++j)
                dpPrev[i][j] = dpCurr[i][j];
    }

    /* sum over all final pairs (i,j) */
    seq_t total = 0;
    for (int i = 0; i < COLS; ++i)
        for (int j = 0; j < COLS; ++j)
            total += dpPrev[i][j];

    memo[n] = total;
    return total;
}
