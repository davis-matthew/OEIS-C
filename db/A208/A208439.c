/*
 * A208439  Number of n X 2 0..2 arrays with no element equal the average of
 *          immediate neighbors vertically above and horizontally left of it.
 * https://oeis.org/A208439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies a(n) = 3*a(n-1) + 10*a(n-2) - 7*a(n-3) + 8*a(n-4)
 *            - 8*a(n-5) computed by dynamic programming over 2×n arrays with
 *            values 0..2 and the restriction that no element equals the
 *            average of its immediate neighbors above and to the left
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Transition rules between successive rows:
   Let prev = (p1,p2), curr = (c1,c2).
   Constraints:
      c1 != p1  (current first column cannot equal the one above)
      if (p2 + c1) is even then c2 != (p2 + c1)/2
      otherwise no restriction on c2.
*/
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined, but safe */
    if (n == 1) return (seq_t)6;   /* 6 arrays of size 1×2 */

    /* Map state index to its column values */
    static const uint8_t st_c1[9] = {0,0,0,1,1,1,2,2,2};
    static const uint8_t st_c2[9] = {0,1,2,0,1,2,0,1,2};

    /* Precompute transition matrix once per process */
    static bool trans[9][9];
    static bool trans_init = false;
    if (!trans_init) {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                uint8_t p1 = st_c1[i], p2 = st_c2[i];
                uint8_t c1 = st_c1[j], c2 = st_c2[j];
                if (c1 == p1) {
                    trans[i][j] = false;
                    continue;
                }
                uint8_t sum = p2 + c1;
                if (!( (sum & 1) == 0 && c2 == sum / 2 )) {
                    trans[i][j] = true;
                } else {
                    trans[i][j] = false;
                }
            }
        }
        trans_init = true;
    }

    /* DP arrays */
    seq_t dp_prev[9] = {0}, dp_curr[9];
    /* Initial row: any state with c2 != c1 is allowed */
    for (int s = 0; s < 9; ++s) {
        if (st_c1[s] != st_c2[s]) dp_prev[s] = (seq_t)1;
    }

    for (idx_t row = 2; row <= n; ++row) {
        for (int s = 0; s < 9; ++s) dp_curr[s] = (seq_t)0;
        for (int p = 0; p < 9; ++p) {
            if (dp_prev[p] == (seq_t)0) continue;
            seq_t val = dp_prev[p];
            for (int c = 0; c < 9; ++c) {
                if (trans[p][c]) {
                    dp_curr[c] += val;
                }
            }
        }
        for (int s = 0; s < 9; ++s) dp_prev[s] = dp_curr[s];
    }

    seq_t ans = (seq_t)0;
    for (int s = 0; s < 9; ++s) ans += dp_prev[s];
    return ans;
}
