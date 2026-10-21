/*
 * A228795  Number of nX7 binary arrays with top left element equal to 1 and
 *          no two ones adjacent horizontally or nw-se.
 * https://oeis.org/A228795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* # Formula: Uses dynamic programming over 7‑bit patterns.
   a(n) counts n×7 binary arrays with (1,1)=1 and no two 1’s
   adjacent horizontally or along the NW‑SE diagonal. */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Pre‑computed list of admissible 7‑bit rows (no horizontal 1‑adjacency). */
    static int masks[34] = {0};
    static bool start_mask[34] = {0};
    static bool trans[34][34] = {0};
    static bool ready = false;

    if (!ready) {
        /* Build all masks and transition table. */
        int count = 0;
        for (int m = 0; m < (1 << 7); ++m) {
            if ((m & (m << 1)) == 0) {          /* no horizontal adjacency */
                masks[count] = m;
                /* first cell must be 1 (bit 0), the second must be 0 by the mask test */
                start_mask[count] = ((m & 1) != 0);
                ++count;
            }
        }
        /* Transition: from pattern p to q allowed if (p << 1) & q == 0
           (no NW‑SE adjacency). */
        for (int p = 0; p < count; ++p) {
            for (int q = 0; q < count; ++q) {
                trans[p][q] = (((masks[p] << 1) & masks[q]) == 0);
            }
        }
        ready = true;
    }

    /* DP vectors: counts of sequences ending in each admissible pattern. */
    seq_t curr[34] = {0};
    seq_t next[34] = {0};

    /* First row. */
    for (int i = 0; i < 34; ++i) {
        if (start_mask[i])
            curr[i] = 1;
    }

    if (n == 1) {
        seq_t sum = 0;
        for (int i = 0; i < 34; ++i)
            sum += curr[i];
        return sum;
    }

    for (idx_t r = 2; r <= n; ++r) {
        /* zero next vector */
        for (int i = 0; i < 34; ++i)
            next[i] = 0;
        /* transition */
        for (int p = 0; p < 34; ++p) {
            seq_t val = curr[p];
            if (val == 0) continue;
            for (int q = 0; q < 34; ++q) {
                if (trans[p][q]) next[q] += val;
            }
        }
        /* swap curr and next */
        for (int i = 0; i < 34; ++i)
            curr[i] = next[i];
    }

    seq_t ans = 0;
    for (int i = 0; i < 34; ++i)
        ans += curr[i];
    return ans;
}
