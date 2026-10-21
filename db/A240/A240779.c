/*
 * A240779  Number of nX4 0..1 arrays with no element equal to fewer vertical
 *          neighbors than horizontal neighbors, with new values 0..1
 *          introduced in row major order.
 * https://oeis.org/A240779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 0..1 arrays with the first cell forced 0
 *            and where for every element the number of equal vertical
 *            neighbours is not less than the number of equal horizontal
 *            neighbours
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   check_row_signed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include standard headers.  The build supplies the types seq_t and idx_t. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/check_row_signed.h"

seq_t a(idx_t n)
{
    /* Handle the singleton case directly. */
    if (n == 1) {
        seq_t cnt = 0;
        for (int row = 0; row < 16; ++row) {
            if (row & 1) continue;                    /* first cell must be 0 */
            if (check_row_signed(-1, row, -1))
                cnt += 1;
        }
        return cnt;
    }

    /* Pre‑compute transition table once per program run. */
    static bool trans[16][16][16] = {0};
    static bool trans_init = false;
    if (!trans_init) {
        for (int ra = 0; ra < 16; ++ra)
            for (int rb = 0; rb < 16; ++rb)
                for (int rc = 0; rc < 16; ++rc)
                    trans[ra][rb][rc] = check_row_signed(ra, rb, rc);
        trans_init = true;
    }

    /* DP arrays.  Index = (prev_row << 4) | cur_row. */
    seq_t cur[256] = {0};
    seq_t nxt[256] = {0};

    /* Initialization: admissible first two rows. */
    for (int r1 = 0; r1 < 16; ++r1) {
        if (r1 & 1) continue;               /* first cell must be 0 */
        for (int r2 = 0; r2 < 16; ++r2) {
            if (check_row_signed(-1, r1, r2))
                cur[(r1 << 4) | r2] += 1;
        }
    }

    /* Build rows 3 .. n. */
    for (idx_t row = 3; row <= n; ++row) {
        for (int i = 0; i < 256; ++i) nxt[i] = 0;
        for (int ra = 0; ra < 16; ++ra) {
            for (int rb = 0; rb < 16; ++rb) {
                seq_t val = cur[(ra << 4) | rb];
                if (!val) continue;
                for (int rc = 0; rc < 16; ++rc) {
                    if (trans[ra][rb][rc])
                        nxt[(rb << 4) | rc] += val;
                }
            }
        }
        for (int i = 0; i < 256; ++i) cur[i] = nxt[i];
    }

    /* Final row check (bottom of the array). */
    seq_t ans = 0;
    for (int ra = 0; ra < 16; ++ra) {
        for (int rb = 0; rb < 16; ++rb) {
            seq_t val = cur[(ra << 4) | rb];
            if (!val) continue;
            if (check_row_signed(ra, rb, -1))
                ans += val;
        }
    }
    return ans;
}
