/*
 * A186058  Number of (n+2) X 6 binary arrays with each 3 X 3 subblock having
 *          a positive determinant.
 * https://oeis.org/A186058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×6 binary arrays with each 3×3 subblock
 *            having a positive determinant
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 19, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#define STATE_COUNT 4096   /* 2^12: two rows of 6 bits each */

/* Build tables of positive determinants for 3×3 binary matrices and
   allowed transitions between pairs of rows */
static void build_tables(bool det_pos[512],
                         uint8_t trans[STATE_COUNT][64],
                         uint8_t trans_cnt[STATE_COUNT])
{
    /* Positive determinant lookup for all 3×3 binaries */
    for (int top = 0; top < 8; ++top)
    for (int mid = 0; mid < 8; ++mid)
    for (int bot = 0; bot < 8; ++bot)
    {
        int a11 = (top >> 0) & 1, a12 = (top >> 1) & 1, a13 = (top >> 2) & 1;
        int a21 = (mid >> 0) & 1, a22 = (mid >> 1) & 1, a23 = (mid >> 2) & 1;
        int a31 = (bot >> 0) & 1, a32 = (bot >> 1) & 1, a33 = (bot >> 2) & 1;
        int det = a11 * (a22 * a33 - a23 * a32)
                - a12 * (a21 * a33 - a23 * a31)
                + a13 * (a21 * a32 - a22 * a31);
        det_pos[(top << 6) | (mid << 3) | bot] = det > 0;
    }

    /* Transition lists for each pair of previous rows */
    for (int state = 0; state < STATE_COUNT; ++state)
    {
        uint8_t prev2 = state >> 6;   /* older of the two rows */
        uint8_t prev1 = state & 63;   /* newer of the two rows */
        uint8_t *out   = trans[state];
        uint8_t  *cnt   = &trans_cnt[state];
        *cnt = 0;

        for (int newRow = 0; newRow < 64; ++newRow)
        {
            bool ok = true;
            for (int j = 0; j < 4; ++j)
            {
                uint8_t topBits = (prev2 >> j) & 7;
                uint8_t midBits = (prev1 >> j) & 7;
                uint8_t botBits = (newRow >> j) & 7;
                if (!det_pos[(topBits << 6) | (midBits << 3) | botBits])
                {
                    ok = false;
                    break;
                }
            }
            if (ok)
                out[(*cnt)++] = (uint8_t)newRow;
        }
    }
}

seq_t a(idx_t n)
{
    /* static tables built once */
    static bool      det_pos[512];
    static uint8_t   trans[STATE_COUNT][64];
    static uint8_t   trans_cnt[STATE_COUNT];
    static bool      init = false;
    if (!init)
    {
        build_tables(det_pos, trans, trans_cnt);
        init = true;
    }

    /* DP arrays holding counts for each pair of last two rows */
    static seq_t curr_arr[STATE_COUNT];
    static seq_t next_arr[STATE_COUNT];

    seq_t *curr = curr_arr;
    seq_t *next = next_arr;

    /* Initially, after the first two rows, every state is possible once */
    for (int i = 0; i < STATE_COUNT; ++i)
        curr[i] = (seq_t)1;

    /* Each additional row imposes the 3×3 determinant condition */
    for (idx_t step = 0; step < n; ++step)
    {
        /* zero next array */
        memset(next, 0, sizeof(next_arr));

        for (int state = 0; state < STATE_COUNT; ++state)
        {
            seq_t ways = curr[state];
            if (!ways) continue;          /* skip states that cannot be reached */

            uint8_t cnt  = trans_cnt[state];
            uint8_t *list = trans[state];

            for (uint8_t k = 0; k < cnt; ++k)
            {
                uint8_t newRow = list[k];
                int newState = ((state & 63) << 6) | newRow;
                next[newState] += ways;
            }
        }

        /* swap current and next arrays */
        seq_t *tmp = curr;
        curr = next;
        next = tmp;
    }

    /* sum all possibilities for the final state */
    seq_t total = 0;
    for (int i = 0; i < STATE_COUNT; ++i)
        total += curr[i];
    return total;
}
