/*
 * A237856  Number of (n+1)X(5+1) 0..1 arrays with no element greater than all
 *          horizontal neighbors or less than all vertical neighbors.
 * https://oeis.org/A237856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes OEIS A237856: Number of (n+1)×6 binary arrays 
   with no element greater than all horizontal neighbors 
   and no element less than all vertical neighbors.
   The algorithm DP‑counts row patterns of length 6. 
*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static initialization of row patterns and transition tables */
    static bool init = false;
    static uint8_t  allowed_masks[21];      /* masks of admissible rows   */
    static int      mask_to_idx[64];        /* lookup mask -> index      */
    static uint8_t  num_allowed = 0;        /* number of admissible rows */

    /* Pre‑computed adjacency lists (prev, curr) -> list of next indices */
    static uint8_t  next_len[21][21];
    static uint8_t  next_list[21][21][21];

    /* Subset lookup: is_sub[i][j]==true if mask[j] ⊆ mask[i]            */
    static bool     is_sub[21][21];

    if (!init) {
        /* Initialize all masks to -1 (invalid) */
        for (int i = 0; i < 64; ++i) mask_to_idx[i] = -1;

        /* Determine admissible 6‑bit masks: no isolated 1 horizontally */
        for (int mask = 0; mask < 64; ++mask) {
            bool ok = true;
            for (int j = 0; j < 6 && ok; ++j) {
                if ((mask >> j) & 1) {
                    bool left  = (j > 0)  && ((mask >> (j-1)) & 1);
                    bool right = (j < 5)  && ((mask >> (j+1)) & 1);
                    if (!left && !right) ok = false;
                }
            }
            if (ok) {
                allowed_masks[num_allowed] = (uint8_t)mask;
                mask_to_idx[mask] = (int)num_allowed;
                ++num_allowed;
            }
        }

        /* Subset table */
        for (int i = 0; i < num_allowed; ++i) {
            uint32_t mi = allowed_masks[i];
            for (int j = 0; j < num_allowed; ++j) {
                uint32_t mj = allowed_masks[j];
                is_sub[i][j] = ((mj & ~mi) == 0);
            }
        }

        /* Pre‑compute transition lists for interior rows */
        for (int pi = 0; pi < num_allowed; ++pi) {
            uint32_t mi = allowed_masks[pi];
            for (int ci = 0; ci < num_allowed; ++ci) {
                uint32_t mj = allowed_masks[ci];
                size_t   len = 0;
                for (int ni = 0; ni < num_allowed; ++ni) {
                    uint32_t mk = allowed_masks[ni];
                    /* Condition: (~mj) & (mi & mk) == 0  ⇔  mi & mk ⊆ mj */
                    if ( ((mi & mk) & (~mj)) == 0 ) {
                        next_list[pi][ci][len++] = (uint8_t)ni;
                    }
                }
                next_len[pi][ci] = (uint8_t)len;
            }
        }
        init = true;
    }

    /* Number of rows = n + 1 (offset 1) */
    idx_t rows = n + 1;

    /* n = 0 not used, but guard anyway */
    if (rows == 0) return (seq_t)0;

    /* Case L = 1 : only horizontal constraint matters */
    if (rows == 1) return (seq_t)num_allowed;

    /* DP arrays for pairs of consecutive rows: [prev][curr] */
    static seq_t dp_cur[21][21];
    static seq_t dp_next[21][21];

    /* Initialize dp_cur for first two rows (top constraint) */
    for (int i = 0; i < num_allowed; ++i)
        for (int j = 0; j < num_allowed; ++j)
            dp_cur[i][j] = 0;

    for (int pi = 0; pi < num_allowed; ++pi) {
        for (int ci = 0; ci < num_allowed; ++ci) {
            if (is_sub[pi][ci])        /* next ⊆ prev  (top row) */
                dp_cur[pi][ci] = 1;
        }
    }

    /* Number of additional rows after the first two */
    idx_t add_rows = rows - 2;
    for (idx_t step = 0; step < add_rows; ++step) {
        /* Zero dp_next */
        for (int i = 0; i < num_allowed; ++i)
            for (int j = 0; j < num_allowed; ++j)
                dp_next[i][j] = 0;

        /* Transition using pre‑computed adjacency */
        for (int pi = 0; pi < num_allowed; ++pi) {
            for (int ci = 0; ci < num_allowed; ++ci) {
                seq_t cur_val = dp_cur[pi][ci];
                if (cur_val == 0) continue;
                size_t len = next_len[pi][ci];
                for (size_t k = 0; k < len; ++k) {
                    int ni = next_list[pi][ci][k];
                    dp_next[ci][ni] += cur_val;
                }
            }
        }

        /* Swap dp_now and dp_next */
        for (int i = 0; i < num_allowed; ++i)
            for (int j = 0; j < num_allowed; ++j)
                dp_cur[i][j] = dp_next[i][j];
    }

    /* Final row is bottom, enforce bottom condition: curr ⊇ prev */
    seq_t result = 0;
    for (int pi = 0; pi < num_allowed; ++pi) {
        for (int ci = 0; ci < num_allowed; ++ci) {
            if (is_sub[ci][pi])      /* prev ⊆ curr  (bottom row) */
                result += dp_cur[pi][ci];
        }
    }

    return result;
}
