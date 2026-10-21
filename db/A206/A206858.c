/*
 * A206858  Number of nX3 0..2 arrays avoiding the pattern z z+1 z in any row,
 *          column, diagonal or antidiagonal.
 * https://oeis.org/A206858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A206858 – Number of n×3 0..2 arrays avoiding the pattern z,z+1,z
 *  in any row, column, diagonal or antidiagonal.
 *
 *  This implementation counts the arrays by dynamic programming over the
 *  last two rows of the array.  There are 25 admissible rows; hence
 *  there are 625 states (row_{n-2},row_{n-1}).  Transition tables are
 *  pre‑computed once.  The sequence is memoised for n ≤ 10 000; larger
 *  arguments are evaluated from scratch.
 *
 *  Complexity: O(n · 625) time and O(625) space (plus memoisation for
 *  n ≤ 10000).  The count fits in the arbitrary‑precision type `seq_t`,
 *  and for 64‑bit builds the result is exact as long as it fits in the
 *  64‑bit type.
 *
 *  External prototype: seq_t a(idx_t n);
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* --------- Parameters and static data --------------------------------- */
#define MAX_ROWS 25                      /* number of legal rows               */
#define STATE_COUNT (MAX_ROWS * MAX_ROWS)/* 625 different states                */
#define MAX_TRANS 25                     /* maximum outgoing transitions per state */

static bool tables_ready = false;

static uint8_t rows[MAX_ROWS][3];          /* digits of each legal row          */
static int    next_state[STATE_COUNT][MAX_TRANS];
static uint8_t next_cnt[STATE_COUNT];

/* --------- Memoisation for n ≤ 10000 ---------------------------------- */
#define MAX_N 10000
static seq_t memo[MAX_N + 1] = {0};        /* memo[n] stores a(n), 1‑based     */
static idx_t  memo_max = 0;                /* largest n already computed        */

/* --------- DP storage ----------------------------------------------- */
static seq_t dp_cur[STATE_COUNT];

/* --------- Build row list and transition tables --------------------- */
static void build_tables(void)
{
    if (tables_ready)
        return;

    /* Enumerate legal rows (exclude forbidden horizontal patterns) */
    int rcount = 0;
    for (int enc = 0; enc < 27; ++enc) {   /* 3³ = 27 possible rows           */
        int d0 = enc % 3;
        int d1 = (enc / 3) % 3;
        int d2 = enc / 9;

        /* skip 0,1,0 and 1,2,1 */
        if ((d0 == 0 && d1 == 1 && d2 == 0) ||
            (d0 == 1 && d1 == 2 && d2 == 1))
            continue;

        rows[rcount][0] = (uint8_t)d0;
        rows[rcount][1] = (uint8_t)d1;
        rows[rcount][2] = (uint8_t)d2;
        rcount++;
    }

    /* Build transition tables */
    for (int s = 0; s < STATE_COUNT; ++s) {
        int r2 = s / MAX_ROWS;   /* previous-previous row             */
        int r1 = s % MAX_ROWS;   /* previous row                       */
        int cnt = 0;
        for (int r0 = 0; r0 < MAX_ROWS; ++r0) {  /* new row                         */
            bool ok = true;

            /* Column checks */
            for (int col = 0; col < 3 && ok; ++col) {
                int a = rows[r2][col];
                int b = rows[r1][col];
                int c = rows[r0][col];
                if ((a == 0 && b == 1 && c == 0) ||
                    (a == 1 && b == 2 && c == 1))
                    ok = false;
            }
            /* Main diagonal: r2[0], r1[1], r0[2] */
            if (ok) {
                int a = rows[r2][0];
                int b = rows[r1][1];
                int c = rows[r0][2];
                if ((a == 0 && b == 1 && c == 0) ||
                    (a == 1 && b == 2 && c == 1))
                    ok = false;
            }
            /* Anti‑diagonal: r2[2], r1[1], r0[0] */
            if (ok) {
                int a = rows[r2][2];
                int b = rows[r1][1];
                int c = rows[r0][0];
                if ((a == 0 && b == 1 && c == 0) ||
                    (a == 1 && b == 2 && c == 1))
                    ok = false;
            }

            if (ok) {
                next_state[s][cnt++] = r1 * MAX_ROWS + r0;  /* new state */
            }
        }
        next_cnt[s] = (uint8_t)cnt;
    }

    tables_ready = true;
}

/* --------- Compute sequence ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                    /* undefined for n<1 */

    if (n == 1)
        return (seq_t)25;
    if (n == 2)
        return (seq_t)625;

    build_tables();

    /* --------- Small n (≤ MAX_N) with memoisation ------------------- */
    if (n <= MAX_N) {
        if (memo[n] != (seq_t)0)
            return memo[n];

        /* initialise memo and dp_cur if first use */
        if (memo_max < 2) {
            memo[1] = (seq_t)25;
            memo[2] = (seq_t)625;
            for (int i = 0; i < STATE_COUNT; ++i)
                dp_cur[i] = (seq_t)1;        /* all 625 states occur once for n=2 */
            memo_max = 2;
        }

        /* extend DP up to n */
        for (idx_t cur = memo_max + 1; cur <= n; ++cur) {
            seq_t next[STATE_COUNT] = {0};

            for (int s = 0; s < STATE_COUNT; ++s) {
                seq_t val = dp_cur[s];
                if (!val)
                    continue;
                uint8_t tcnt = next_cnt[s];
                for (int j = 0; j < tcnt; ++j) {
                    int t = next_state[s][j];
                    next[t] += val;
                }
            }

            /* sum to get a(cur) */
            seq_t sum = 0;
            for (int i = 0; i < STATE_COUNT; ++i)
                sum += next[i];

            memo[cur] = sum;
            memo_max = cur;
            for (int i = 0; i < STATE_COUNT; ++i)
                dp_cur[i] = next[i];
        }

        return memo[n];
    }

    /* --------- Large n (> MAX_N) evaluated from scratch ---------------- */
    /* Start DP with n=2 state counts all 1 */
    seq_t cur_state[STATE_COUNT];
    for (int i = 0; i < STATE_COUNT; ++i)
        cur_state[i] = (seq_t)1;

    for (idx_t cur = 3; cur <= n; ++cur) {
        seq_t next_state_local[STATE_COUNT] = {0};

        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = cur_state[s];
            if (!val)
                continue;
            uint8_t tcnt = next_cnt[s];
            for (int j = 0; j < tcnt; ++j) {
                int t = next_state[s][j];
                next_state_local[t] += val;
            }
        }

        /* move to next row */
        for (int i = 0; i < STATE_COUNT; ++i)
            cur_state[i] = next_state_local[i];
    }

    /* sum of resulting states */
    seq_t sum = 0;
    for (int i = 0; i < STATE_COUNT; ++i)
        sum += cur_state[i];

    return sum;
}
