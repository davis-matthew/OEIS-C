/*
 * A025588  Number of n-move self-avoiding knight paths on 4 X 4 board,
 *          beginning at corner.
 * https://oeis.org/A025588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumeration of self‑avoiding knight paths on a 4×4 board
   starting from the corner (0,0).  a(n) is the number of distinct
   n‑move paths that never revisit a square.  The sequence becomes
   zero after the maximum possible length (15).  The function
   generates the values on first call and memoises them for
   subsequent calls.  It works for any seq_t and idx_t supplied by
   the build. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;
static seq_t *seq_ans = NULL;
static idx_t ans_len = 0;
static bool initialized = false;

static const int knight_dx[8] = { 1,  2, -1, -2,  1,  2, -1, -2 };
static const int knight_dy[8] = { 2,  1,  2,  1, -2, -1, -2, -1 };

seq_t a(idx_t n)
{
    if (!initialized) {
        /* Number of states: 2^16 masks × 16 positions = 1,048,576 */
        const size_t MASKS  = 1U << 16;
        const size_t STATES = MASKS * 16;

        /* Allocate two tables for one‑step DP */
        dp_cur   = (seq_t *)malloc(STATES * sizeof(seq_t));
        dp_next  = (seq_t *)malloc(STATES * sizeof(seq_t));
        if (!dp_cur || !dp_next) return (seq_t)0;

        /* Zero the tables.  Use size_t for loop bounds. */
        for (size_t i = 0; i < STATES; ++i) {
            dp_cur[i]  = 0;
            dp_next[i] = 0;
        }

        /* Initial state: path of length 0 at corner (0,0).  Its mask
           has bit 0 set. */
        dp_cur[ ((size_t)1U << 4) + 0 ] = 1;   /* mask 1 << 0 = 1 */
        ans_len = 1;
        seq_ans = (seq_t *)malloc(ans_len * sizeof(seq_t));
        seq_ans[0] = (seq_t)1;

        /* Generate successive layers until no paths remain. */
        for (uint32_t step = 0; ; ++step) {
            /* Reset dp_next */
            for (size_t i = 0; i < STATES; ++i) dp_next[i] = 0;

            seq_t sum = 0;

            for (uint32_t mask = 0; mask < MASKS; ++mask) {
                size_t base = ((size_t)mask) << 4;          /* mask*16 */
                for (int pos = 0; pos < 16; ++pos) {
                    seq_t cnt = dp_cur[base + pos];
                    if (!cnt) continue;
                    int x = pos & 3;
                    int y = pos >> 2;
                    for (int dir = 0; dir < 8; ++dir) {
                        int nx = x + knight_dx[dir];
                        int ny = y + knight_dy[dir];
                        if (nx < 0 || nx >= 4 || ny < 0 || ny >= 4) continue;
                        int newpos = nx + ny * 4;
                        if (mask & (1U << newpos)) continue;  /* visited before */
                        uint32_t newMask = mask | (1U << newpos);
                        size_t idx = (((size_t)newMask) << 4) + newpos;
                        dp_next[idx] += cnt;
                        sum += cnt;
                    }
                }
            }

            /* Store count for this length */
            seq_ans = (seq_t *)realloc(seq_ans, (ans_len + 1) * sizeof(seq_t));
            seq_ans[ans_len++] = sum;

            /* If no paths remain, break.  The next count will be zero. */
            if (sum == 0) break;

            /* Prepare for next step */
            seq_t *tmp = dp_cur;  dp_cur = dp_next; dp_next = tmp;
        }
        initialized = true;
    }

    if (n < ans_len) return seq_ans[(size_t)n];
    return (seq_t)0;
}
