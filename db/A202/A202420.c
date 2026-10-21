/*
 * A202420  Number of (n+2)X9 binary arrays with no more than one of any
 *          consecutive three bits set in any row or column.
 * https://oeis.org/A202420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes A202420: the number of (n+2)×9 binary arrays
   with no more than one of any consecutive three bits set in any row
   or column. It uses dynamic programming over admissible 9‑bit row
   patterns. */

/* include types provided by the build system */
#include <stdint.h>
#include <stdbool.h>

/* Max possible number of admissible 9‑bit masks (we only need 41) */
#define MAX_MASKS 64

seq_t a(idx_t n)
{
    /* List of admissible 9‑bit masks.
       A mask is admissible if it contains no two 1‑bits with distance
       ≤ 2 (i.e. no pattern 11 or 101). */
    static uint16_t masks[MAX_MASKS];
    static int num_masks = 0;

    /* Transition table: for each pair of previous masks (p2,p1)
       store indices of admissible next masks and the number of
       such transitions. */
    static int trans_cnt[MAX_MASKS][MAX_MASKS];
    static uint16_t trans_list[MAX_MASKS][MAX_MASKS][MAX_MASKS];

    /* DP state arrays.  dp_cur[p2][p1] stores the count of arrays
       that end with previous rows having masks p2 (row r‑2) and
       p1 (row r‑1). */
    static seq_t dp_cur[MAX_MASKS][MAX_MASKS];
    static seq_t dp_nxt[MAX_MASKS][MAX_MASKS];

    /* One‑time initialization. */
    static bool initialized = false;
    if (!initialized) {
        /* generate all admissible masks */
        for (int m = 0; m < 512; ++m) {
            if (((m & (m << 1)) == 0) && ((m & (m << 2)) == 0)) {
                masks[num_masks++] = (uint16_t)m;
            }
        }

        /* pre‑compute transitions */
        for (int p2 = 0; p2 < num_masks; ++p2) {
            for (int p1 = 0; p1 < num_masks; ++p1) {
                int count = 0;
                for (int mi = 0; mi < num_masks; ++mi) {
                    uint16_t m  = masks[mi];
                    if ((m & masks[p1]) == 0 && (m & masks[p2]) == 0) {
                        trans_list[p2][p1][count++] = (uint16_t)mi;
                    }
                }
                trans_cnt[p2][p1] = count;
            }
        }
        initialized = true;
    }

    /* initialize DP; two empty rows before the first real row */
    for (int i = 0; i < num_masks; ++i)
        for (int j = 0; j < num_masks; ++j)
            dp_cur[i][j] = 0;
    dp_cur[0][0] = 1;  /* mask 0 is stored at index 0 */

    idx_t rows = n + 2;   /* number of rows in the matrix */
    for (idx_t r = 0; r < rows; ++r) {
        /* clear next DP array */
        for (int i = 0; i < num_masks; ++i)
            for (int j = 0; j < num_masks; ++j)
                dp_nxt[i][j] = 0;

        /* transition */
        for (int p2 = 0; p2 < num_masks; ++p2) {
            for (int p1 = 0; p1 < num_masks; ++p1) {
                seq_t val = dp_cur[p2][p1];
                if (!val) continue;
                int cnt = trans_cnt[p2][p1];
                for (int k = 0; k < cnt; ++k) {
                    uint16_t mi = trans_list[p2][p1][k];
                    dp_nxt[p1][mi] += val;
                }
            }
        }

        /* move to next row */
        for (int i = 0; i < num_masks; ++i)
            for (int j = 0; j < num_masks; ++j)
                dp_cur[i][j] = dp_nxt[i][j];
    }

    /* sum all final states */
    seq_t result = 0;
    for (int i = 0; i < num_masks; ++i)
        for (int j = 0; j < num_masks; ++j)
            result += dp_cur[i][j];

    return result;
}
