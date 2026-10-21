/*
 * A207758  Number of nX4 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A207758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   dynamic programming over the 4‑bit rows. a(1)=9, a(2)=81; for
 *            n≥3, let T_i,j,k be true iff rows i,j,k satisfy the vertical
 *            restrictions; then a(n)=∑_{i,j} dp_{n}[i][j] with dp recurrence:
 *            dp_2[i][j]=1, dp_l+1[j][k]=∑_{i} dp_l[i][j]·T_i,j,k
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 20, u64 n <= 44, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* allowed 4‑bit rows (no horizontal 000 or 101) */
    static const unsigned int allowed[9] = {2,3,4,6,7,9,12,14,15};

    /* pre‑compute vertical transitions for any triple of rows */
    static bool trans[9][9][9] = {0};
    static bool init = false;

    if (!init) {
        for (int i = 0; i < 9; ++i) {
            unsigned int r1 = allowed[i];
            for (int j = 0; j < 9; ++j) {
                unsigned int r2 = allowed[j];
                for (int k = 0; k < 9; ++k) {
                    unsigned int r3 = allowed[k];
                    bool ok = true;
                    for (int col = 0; col < 4; ++col) {
                        int b1 = (r1 >> col) & 1;
                        int b2 = (r2 >> col) & 1;
                        int b3 = (r3 >> col) & 1;
                        if ((b1 == 0 && b2 == 1 && b3 == 1) ||
                            (b1 == 1 && b2 == 0 && b3 == 1)) {
                            ok = false;
                            break;
                        }
                    }
                    trans[i][j][k] = ok;
                }
            }
        }
        init = true;
    }

    if (n == 1) return 9;
    if (n == 2) return 81;

    seq_t dp[9][9];
    seq_t nextdp[9][9];

    /* base: all pairs of allowed rows are valid for length 2 */
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            dp[i][j] = 1;

    for (idx_t step = 3; step <= n; ++step) {
        /* zero nextdp */
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                nextdp[i][j] = 0;

        /* apply transitions */
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                seq_t cnt = dp[i][j];
                if (cnt == 0) continue;
                for (int k = 0; k < 9; ++k) {
                    if (trans[i][j][k]) {
                        nextdp[j][k] += cnt;
                    }
                }
            }
        }

        /* move nextdp → dp */
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                dp[i][j] = nextdp[i][j];
    }

    /* sum all states for length n */
    seq_t sum = 0;
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            sum += dp[i][j];

    return sum;
}
