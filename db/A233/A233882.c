/*
 * A233882  Number of (n+1)X(7+1) 0..2 arrays with every 2X2 subblock having
 *          the sum of the squares of all six edge and diagonal differences
 *          equal to 11.
 * https://oeis.org/A233882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   alloc_seq, set_row_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = number of (n+1)×8 arrays over {0,1,2}
 * such that every 2×2 subblock has the sum of squares of all six edge
 * and diagonal differences equal to 11.
 * This is computed by dynamic programming over the 3^8 = 6561 possible
 * row states, counting transitions that satisfy the 2×2 condition. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_seq.h"   /* gives seq_t *alloc_seq(size_t) */
#include "../utils/set_row_digits.h"

/* 3^8 = 6561 possible rows */
#define TOTAL_STATES 6561

/* Test if two rows can form a valid 2×2 block for all 7 positions. */
static bool rows_compatible(const int *r, const int *s)
{
    for (int j = 0; j < 7; ++j) {
        int d1 = r[j] - r[j + 1];
        int d2 = s[j] - s[j + 1];
        int d3 = r[j] - s[j];
        int d4 = r[j + 1] - s[j + 1];
        int d5 = r[j] - s[j + 1];
        int d6 = r[j + 1] - s[j];
        int sum = d1 * d1 + d2 * d2 + d3 * d3 + d4 * d4 + d5 * d5 + d6 * d6;
        if (sum != 11) return false;
    }
    return true;
}

/* Adjacency list of valid next rows. */
struct Adjacency {
    int count;
    int *list;
};

static int row_digits[TOTAL_STATES][8];
static struct Adjacency adj[TOTAL_STATES];
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;
static bool initialized = false;

/* Build adjacency lists and allocate DP arrays. */
static void init_data(void)
{
    /* Pre‑compute digits for every possible row. */
    for (int state = 0; state < TOTAL_STATES; ++state)
        set_row_digits(state, row_digits[state]);

    /* Build adjacency lists. */
    for (int r = 0; r < TOTAL_STATES; ++r) {
        int cnt = 0;
        for (int s = 0; s < TOTAL_STATES; ++s)
            if (rows_compatible(row_digits[r], row_digits[s]))
                ++cnt;
        adj[r].count = cnt;
        adj[r].list = (int *)malloc(cnt * sizeof(int));
        int idx = 0;
        for (int s = 0; s < TOTAL_STATES; ++s)
            if (rows_compatible(row_digits[r], row_digits[s]))
                adj[r].list[idx++] = s;
    }

    dp_cur  = alloc_seq(TOTAL_STATES);
    dp_next = alloc_seq(TOTAL_STATES);
}

/* Return a(n) for n ≥ 1. For n == 0 the value is defined as 0. */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_data();
        initialized = true;
    }

    if (n == 0) return (seq_t)0;

    /* Start with one row: each state occurs once. */
    for (int i = 0; i < TOTAL_STATES; ++i)
        dp_cur[i] = (seq_t)1;

    /* Perform n steps to build arrays of length n+1 rows. */
    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < TOTAL_STATES; ++i)
            dp_next[i] = (seq_t)0;

        for (int r = 0; r < TOTAL_STATES; ++r) {
            seq_t ways = dp_cur[r];
            if (ways == (seq_t)0) continue;
            int *nb = adj[r].list;
            int cnt = adj[r].count;
            for (int k = 0; k < cnt; ++k)
                dp_next[nb[k]] += ways;
        }

        /* swap current and next arrays */
        seq_t *tmp = dp_cur;
        dp_cur   = dp_next;
        dp_next  = tmp;
    }

    /* Sum over all ending rows. */
    seq_t answer = (seq_t)0;
    for (int i = 0; i < TOTAL_STATES; ++i)
        answer += dp_cur[i];

    return answer;
}
