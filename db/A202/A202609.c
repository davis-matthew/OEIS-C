/*
 * A202609  Number of (n+2) X 3 binary arrays avoiding patterns 001 and 010 in
 *          rows and columns.
 * https://oeis.org/A202609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×3 binary arrays avoiding patterns 001 and
 *            010 in rows and columns. The algorithm uses dynamic programming
 *            over the rows. A state consists of the last two rows (each a
 *            3‑bit pattern). The 6 allowed row patterns are
 *            {000,011,100,101,110,111}. A transition from rows (prev, cur...
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 17, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;        /* undefined for n<1, safe fallback */

    /* Map the six allowed 3‑bit rows to integer values 0 … 7. */
    static const int allowed[6] = {0, 3, 4, 5, 6, 7};

    /* Precompute valid transitions between consecutive rows. */
    static bool trans[6][6][6];
    static bool trans_init = false;
    if (!trans_init) {
        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j < 6; ++j) {
                for (int k = 0; k < 6; ++k) {
                    bool ok = true;
                    int a = allowed[i];
                    int b = allowed[j];
                    int c = allowed[k];
                    for (int col = 0; col < 3; ++col) {
                        int x = (a >> col) & 1;
                        int y = (b >> col) & 1;
                        int z = (c >> col) & 1;
                        if ((x == 0 && y == 0 && z == 1) ||
                            (x == 0 && y == 1 && z == 0)) {
                            ok = false;
                            break;
                        }
                    }
                    trans[i][j][k] = ok;
                }
            }
        }
        trans_init = true;
    }

    /* Number of rows in the array */
    size_t rows = (size_t)(n + 2);

    /* DP array for the last two rows: dp[i][j] = count of arrays ending with
       rows allowed[i] and allowed[j] as the last two rows. */
    seq_t dp[6][6]  = {0};
    seq_t newdp[6][6] = {0};

    /* Initialisation: choose any two allowed rows for the first two rows. */
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 6; ++j)
            dp[i][j] = (seq_t)1;

    /* Process additional rows, one at a time. */
    for (size_t t = 3; t <= rows; ++t) {
        /* zero newdp */
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < 6; ++j)
                newdp[i][j] = (seq_t)0;

        /* transition */
        for (int prev = 0; prev < 6; ++prev) {
            for (int curr = 0; curr < 6; ++curr) {
                seq_t val = dp[prev][curr];
                if (val == (seq_t)0) continue;
                for (int nxt = 0; nxt < 6; ++nxt) {
                    if (trans[prev][curr][nxt]) {
                        newdp[curr][nxt] += val;
                    }
                }
            }
        }
        /* swap dp and newdp */
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < 6; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* Sum all ending states. */
    seq_t result = (seq_t)0;
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 6; ++j)
            result += dp[i][j];

    return result;
}
