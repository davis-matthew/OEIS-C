/*
 * A221452  Number of 7 X n binary arrays with every 0 a horizontal or
 *          antidiagonal neighbor to some 1.
 * https://oeis.org/A221452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 7×n binary arrays in which every 0 has a 1
 *            horizontal neighbor (left/right) or an anti‑diagonal neighbor
 *            (top‑right/bottom‑left). Computed by a transfer‑matrix DP over
 *            column masks, keeping a “need” mask of zeros that must be
 *            satisfied by the next column. Complexity O(n·S·C) wher...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Precomputation once per program execution */
    static bool init = false;
    static uint8_t satisfy_mask[128];
    static uint8_t new_need[128][128];
    static uint8_t cand_list[128][128];
    static uint8_t cand_count[128];

    if (!init) {
        /* satisfy_mask[mask] = rows r where mask has a 1 in row r
           or in row r-1 – needed to satisfy a zero requiring a
           right or a top‑right neighbor. */
        for (int mask = 0; mask < 128; ++mask) {
            uint8_t tmp = (uint8_t)mask;
            tmp |= (uint8_t)(tmp << 1);
            satisfy_mask[mask] = tmp & 0x7F;
        }

        /* new_need[prev][curr] = zeros in curr that have no left
           or bottom‑left neighbour in prev, thus need a future 1. */
        for (int prev = 0; prev < 128; ++prev) {
            uint8_t left_down = (uint8_t)prev | (uint8_t)(prev >> 1);
            for (int curr = 0; curr < 128; ++curr) {
                uint8_t need = ((uint8_t)~curr) & ((uint8_t)~left_down) & 0x7F;
                new_need[prev][curr] = need;
            }
        }

        /* For each need mask, list all curr masks that make it
           satisfied.  A need mask is satisfied iff all its bits
           are present in satisfy_mask[curr]. */
        for (int need = 0; need < 128; ++need) {
            uint8_t count = 0;
            for (int curr = 0; curr < 128; ++curr) {
                if ((need & (uint8_t)~satisfy_mask[curr]) == 0) {
                    cand_list[need][count++] = (uint8_t)curr;
                }
            }
            cand_count[need] = count;
        }
        init = true;
    }

    /* DP arrays: state id = (prev_mask << 7) | need_mask, size 128*128 = 16384 */
    static seq_t dp[16384];
    static seq_t ndp[16384];
    memset(dp, 0, sizeof(dp));
    dp[0] = (seq_t)1;                     /* prev_mask = 0, need = 0 */

    for (idx_t col = 0; col < n; ++col) {
        memset(ndp, 0, sizeof(ndp));

        for (int sid = 0; sid < 16384; ++sid) {
            seq_t ways = dp[sid];
            if (!ways) continue;

            uint8_t prev_mask = (uint8_t)(sid >> 7);
            uint8_t need = (uint8_t)(sid & 0x7F);

            /* Iterate over all curr masks that satisfy this need */
            uint8_t *list = cand_list[need];
            uint8_t list_len = cand_count[need];
            for (uint8_t li = 0; li < list_len; ++li) {
                uint8_t curr = list[li];
                uint8_t new_needs = new_need[prev_mask][curr];
                int nsid = (curr << 7) | new_needs;
                ndp[nsid] += ways;
            }
        }
        /* swap dp and ndp */
        memcpy(dp, ndp, sizeof(dp));
    }

    /* Sum over states whose need mask is zero (no future requirement). */
    seq_t result = (seq_t)0;
    for (int mask = 0; mask < 128; ++mask) {
        int sid = (mask << 7);          /* need == 0 */
        result += dp[sid];
    }
    return result;
}
