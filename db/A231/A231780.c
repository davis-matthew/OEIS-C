/*
 * A231780  Number of nX3 0..2 arrays with no element having a strict majority
 *          of its horizontal and vertical neighbors equal to itself plus one
 *          mod 3, with upper left element zero (rock paper and scissors drawn
 *          positions).
 * https://oeis.org/A231780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A231780: number of n×3 arrays with entries 0..2, upper-left fixed 0,
   and no element having a strict majority of its horizontal and vertical
   neighbors equal to (value+1) mod 3.
   Computed by dynamic programming over the rows of width 3. */
#include <stdbool.h>
#include <stddef.h>

static int row_vals[27][3];
static bool valid_single[27];
static bool valid_top[27][27];
static bool valid_bottom[27][27];
static bool valid_triple[27][27][27];
static bool precomputed = false;

static bool row_satisfied(int above_id, int cur_id, int below_id,
                          bool has_above, bool has_below)
{
    const int *cur  = row_vals[cur_id];
    const int *abv  = has_above ? row_vals[above_id] : NULL;
    const int *blw  = has_below ? row_vals[below_id] : NULL;

    for (int c = 0; c < 3; ++c) {
        int val    = cur[c];
        int target = (val + 1) % 3;
        int neigh  = 0;
        int equal  = 0;

        if (c > 0) { ++neigh; if (cur[c-1] == target) ++equal; }
        if (c < 2) { ++neigh; if (cur[c+1] == target) ++equal; }
        if (has_above) { ++neigh; if (abv[c] == target) ++equal; }
        if (has_below) { ++neigh; if (blw[c] == target) ++equal; }

        if (2 * equal > neigh) return false;
    }
    return true;
}

static void precompute(void)
{
    for (int id = 0; id < 27; ++id) {
        row_vals[id][0] = (id / 9) % 3;
        row_vals[id][1] = (id / 3) % 3;
        row_vals[id][2] = id % 3;
    }

    for (int id = 0; id < 27; ++id)
        valid_single[id] = row_satisfied(0, id, 0, false, false);

    for (int r0 = 0; r0 < 27; ++r0)
        for (int r1 = 0; r1 < 27; ++r1)
            valid_top[r0][r1] = row_satisfied(0, r0, r1, false, true);

    for (int a = 0; a < 27; ++a)
        for (int b = 0; b < 27; ++b)
            valid_bottom[a][b] = row_satisfied(a, b, 0, true, false);

    for (int a = 0; a < 27; ++a)
        for (int b = 0; b < 27; ++b)
            for (int c = 0; c < 27; ++c)
                valid_triple[a][b][c] = row_satisfied(a, b, c, true, true);

    precomputed = true;
}

seq_t a(idx_t n)
{
    if (!precomputed) precompute();

    if (n == 0) return (seq_t)0;
    if (n == 1) {
        seq_t count = 0;
        for (int id = 0; id < 27; ++id)
            if (row_vals[id][0] == 0 && valid_single[id])
                count = count + 1;
        return count;
    }

    static seq_t dp_cur[27][27];
    static seq_t dp_next[27][27];

    for (int i = 0; i < 27; ++i)
        for (int j = 0; j < 27; ++j)
            dp_cur[i][j] = 0;

    for (int r0 = 0; r0 < 27; ++r0) {
        if (row_vals[r0][0] != 0) continue;
        for (int r1 = 0; r1 < 27; ++r1) {
            if (valid_top[r0][r1])
                dp_cur[r0][r1] = dp_cur[r0][r1] + 1;
        }
    }

    for (idx_t row = 2; row < n; ++row) {
        for (int i = 0; i < 27; ++i)
            for (int j = 0; j < 27; ++j)
                dp_next[i][j] = 0;

        for (int above = 0; above < 27; ++above) {
            for (int cur = 0; cur < 27; ++cur) {
                seq_t ways = dp_cur[above][cur];
                if (!ways) continue;
                for (int below = 0; below < 27; ++below) {
                    if (!valid_triple[above][cur][below]) continue;
                    dp_next[cur][below] = dp_next[cur][below] + ways;
                }
            }
        }

        for (int i = 0; i < 27; ++i)
            for (int j = 0; j < 27; ++j)
                dp_cur[i][j] = dp_next[i][j];
    }

    seq_t result = 0;
    for (int above = 0; above < 27; ++above)
        for (int cur = 0; cur < 27; ++cur) {
            seq_t ways = dp_cur[above][cur];
            if (!ways) continue;
            if (valid_bottom[above][cur])
                result = result + ways;
        }

    return result;
}
