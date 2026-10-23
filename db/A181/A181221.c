/*
 * A181221  Number of nX5 binary matrices with no three 1's adjacent in a line
 *          horizontally, vertically, diagonally or antidiagonally.
 * https://oeis.org/A181221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary matrices with no three consecutive
 *            1’s in any straight line (horizontal, vertical, or diagonal)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define MASKS 32        /* 5-bit masks */
#define MAX_GOOD 32     /* maximum masks satisfying horizontal constraint */

static uint8_t good_masks[MAX_GOOD];
static uint8_t good_mask_count;
static uint8_t trans_count[MASKS][MASKS];
static uint8_t trans_masks[MASKS][MASKS][MASKS]; /* at most 32 per pair */

static bool initialized = false;

/* Check whether a mask has any three consecutive 1's horizontally */
static inline bool horizontal_ok(int mask)
{
    for (int j = 0; j <= 2; ++j)
        if ((mask & (7 << j)) == (7 << j))   /* 111 at positions j..j+2 */
            return false;
    return true;
}

/* Check whether a transition from (pre2, pre1) to cur is allowed.
   pre2 and pre1 are the two previous rows, cur is the new row. */
static inline bool valid_transition(int pre2, int pre1, int cur)
{
    /* vertical triple */
    if (pre2 & pre1 & cur)
        return false;

    /* diagonal ascending (top-left to bottom-right) */
    for (int j = 2; j < 5; ++j) {   /* current column j */
        if ((pre2 >> (j - 2) & 1) &&
            (pre1 >> (j - 1) & 1) &&
            (cur >> j           & 1))
            return false;
    }

    /* diagonal descending (top-right to bottom-left) */
    for (int j = 0; j < 3; ++j) {   /* current column j */
        if ((pre2 >> (j + 2) & 1) &&
            (pre1 >> (j + 1) & 1) &&
            (cur >> j          & 1))
            return false;
    }

    return true;
}

/* One-time initialization of tables */
static void init_tables(void)
{
    if (initialized) return;
    good_mask_count = 0;
    for (int mask = 0; mask < MASKS; ++mask) {
        if (horizontal_ok(mask))
            good_masks[good_mask_count++] = (uint8_t)mask;
    }

    for (int pre2 = 0; pre2 < MASKS; ++pre2) {
        for (int pre1 = 0; pre1 < MASKS; ++pre1) {
            uint8_t cnt = 0;
            for (int g = 0; g < good_mask_count; ++g) {
                int cur = good_masks[g];
                if (valid_transition(pre2, pre1, cur))
                    trans_masks[pre2][pre1][cnt++] = (uint8_t)cur;
            }
            trans_count[pre2][pre1] = cnt;
        }
    }

    initialized = true;
}

/* Main function required by the challenge */
seq_t a(idx_t n)
{
    init_tables();

    if (n == 1) return (seq_t)good_mask_count;

    static seq_t dp_prev[MASKS][MASKS];
    static seq_t dp_next[MASKS][MASKS];

    /* Zero dp_prev */
    for (int i = 0; i < MASKS; ++i)
        for (int j = 0; j < MASKS; ++j)
            dp_prev[i][j] = (seq_t)0;

    /* States after first row: (prev2=0, prev1=mask) */
    for (int g = 0; g < good_mask_count; ++g) {
        int mask = good_masks[g];
        dp_prev[0][mask] = (seq_t)1;
    }

    /* Iterate rows 2 .. n */
    for (idx_t row = 2; row <= n; ++row) {
        /* Zero dp_next */
        for (int i = 0; i < MASKS; ++i)
            for (int j = 0; j < MASKS; ++j)
                dp_next[i][j] = (seq_t)0;

        for (int pre2 = 0; pre2 < MASKS; ++pre2) {
            for (int pre1 = 0; pre1 < MASKS; ++pre1) {
                seq_t ways = dp_prev[pre2][pre1];
                if (!ways) continue;
                uint8_t cnt = trans_count[pre2][pre1];
                for (uint8_t t = 0; t < cnt; ++t) {
                    int cur = trans_masks[pre2][pre1][t];
                    dp_next[pre1][cur] += ways;
                }
            }
        }

        /* swap dp_prev and dp_next */
        for (int i = 0; i < MASKS; ++i)
            for (int j = 0; j < MASKS; ++j)
                dp_prev[i][j] = dp_next[i][j];
    }

    /* Sum all states after last row */
    seq_t total = (seq_t)0;
    for (int i = 0; i < MASKS; ++i)
        for (int j = 0; j < MASKS; ++j)
            total += dp_prev[i][j];

    return total;
}
