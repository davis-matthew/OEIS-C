/*
 * A207956  Number of nX4 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A207956: Count n×4 binary matrices avoiding horizontal 000 & 101 and vertical 001 & 100.
   Computed with a simple DP over row pairs. */
#include <stdbool.h>

static const int goodRows[9] = { 2, 3, 4, 6, 7, 9, 12, 14, 15 }; /* binary rows of length 4 with no horizontal 000/101 */

static bool allowedTriple[9][9][9]; /* cached compatibility of three consecutive rows */
static bool init_allowed = false;

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!init_allowed) {
        /* Precompute allowed triples */
        for (int a = 0; a < 9; ++a) {
            for (int b = 0; b < 9; ++b) {
                for (int c = 0; c < 9; ++c) {
                    bool ok = true;
                    for (int col = 0; col < 4 && ok; ++col) {
                        int bitA = (goodRows[a] >> (3 - col)) & 1;
                        int bitB = (goodRows[b] >> (3 - col)) & 1;
                        int bitC = (goodRows[c] >> (3 - col)) & 1;
                        if ((bitA == 0 && bitB == 0 && bitC == 1) ||
                            (bitA == 1 && bitB == 0 && bitC == 0))
                            ok = false;
                    }
                    allowedTriple[a][b][c] = ok;
                }
            }
        }
        init_allowed = true;
    }

    if (n == 1) return 9;
    if (n == 2) return 81;

    seq_t cur[81] = {0};
    seq_t nxt[81] = {0};

    /* initial second row: any pair of good rows */
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            cur[i * 9 + j] = 1;

    for (idx_t row = 3; row <= n; ++row) {
        /* clear next rows */
        for (int k = 0; k < 81; ++k) nxt[k] = 0;

        for (int a = 0; a < 9; ++a) {
            for (int b = 0; b < 9; ++b) {
                seq_t val = cur[a * 9 + b];
                if (!val) continue;
                for (int c = 0; c < 9; ++c) {
                    if (allowedTriple[a][b][c])
                        nxt[b * 9 + c] += val;
                }
            }
        }

        /* copy nxt to cur for next iteration */
        for (int k = 0; k < 81; ++k) cur[k] = nxt[k];
    }

    /* sum all states to get total count */
    seq_t total = 0;
    for (int k = 0; k < 81; ++k) total += cur[k];
    return total;
}
