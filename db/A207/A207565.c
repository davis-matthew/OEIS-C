/*
 * A207565  Number of 4Xn 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A207565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=9, a(2)=81, for n≥3 compute by a transfer-matrix DP.
 *            Allowed 4×1 column patterns (4 bits, LSB = bottom row):
 *            0000,0001,0010,0100,0101,1000,1001,1010,1111 – 9 patterns
 *            Transition rule: for a pair (prev2,prev1) and new column newcol,
 *            the triple (bit_i(prev2),bit_i(prev1),bit_i(newcol)) may not...
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 43, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* allowed column bits (LSB = bottom row) */
    static const unsigned char cols[9] = {
        0b0000,
        0b0001,
        0b0010,
        0b0100,
        0b0101,
        0b1000,
        0b1001,
        0b1010,
        0b1111
    };

    /* base cases */
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)81;

    /* precompute transition validity table (once) */
    static bool valid[9][9][9];
    static bool initialized = false;
    if (!initialized) {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                for (int k = 0; k < 9; ++k) {
                    bool ok = true;
                    for (int r = 0; r < 4; ++r) {
                        int a = (cols[i] >> r) & 1;
                        int b = (cols[j] >> r) & 1;
                        int c = (cols[k] >> r) & 1;
                        if ((a == 0 && b == 0 && c == 1) ||
                            (a == 0 && b == 1 && c == 0)) {
                            ok = false;
                            break;
                        }
                    }
                    valid[i][j][k] = ok;
                }
            }
        }
        initialized = true;
    }

    /* DP tables: cur[i][j] = number of sequences ending with columns (i,j) */
    seq_t cur[9][9] = {{0}};
    seq_t nxt[9][9] = {{0}};

    /* initial length 2: all 9×9 pairs allowed */
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            cur[i][j] = (seq_t)1;

    /* iterate columns 3..n */
    for (idx_t len = 2; len < n; ++len) {
        /* zero next */
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                nxt[i][j] = 0;

        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                seq_t v = cur[i][j];
                if (v == 0) continue;
                for (int k = 0; k < 9; ++k) {
                    if (valid[i][j][k]) {
                        nxt[j][k] += v;
                    }
                }
            }
        }

        /* copy next to cur */
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                cur[i][j] = nxt[i][j];
    }

    /* sum all states for length n */
    seq_t total = 0;
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            total += cur[i][j];

    return total;
}
