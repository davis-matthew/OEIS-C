/*
 * A209651  Number of 4Xn 0..1 arrays avoiding 0 0 1 and 1 0 0 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A209651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A209651
    Sequence counts 4×n 0–1 arrays avoiding
        horizontally: 0 0 1 and 1 0 0
        vertically:   0 0 1 and 1 0 1
    The code implements a linear‑time DP using the
    8 allowed column patterns.  No hard‑coded table of values.
*/
#include <stdint.h>
#include <string.h>  /* for memset, if needed */

static const int ALLOWED[8] = {
    0x0,  /* 0000 */
    0x4,  /* 0100 */
    0x6,  /* 0110 */
    0x7,  /* 0111 */
    0x8,  /* 1000 */
    0xC,  /* 1100 */
    0xE,  /* 1110 */
    0xF   /* 1111 */
};

/* Helper to extract a bit of a column state for a given row */
static inline int bit(int state, int row) {
    return (ALLOWED[state] >> row) & 1;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset is 1 */

    /* Special case for n = 1 and n = 2   */
    if (n == 1) return (seq_t)8;   /* 8 allowed columns */
    if (n == 2) return (seq_t)64;  /* 8^2 combinations */

    /* Transition tables, built once per program run */
    static int transNext[64][8];
    static int transCount[64];
    static int trans_inited = 0;
    if (!trans_inited) {
        for (int p = 0; p < 8; ++p) {
            for (int q = 0; q < 8; ++q) {
                int base = p * 8 + q;
                int cnt = 0;
                for (int r = 0; r < 8; ++r) {
                    int ok = 1;
                    for (int row = 0; row < 4; ++row) {
                        int a = bit(p, row);
                        int b = bit(q, row);
                        int c = bit(r, row);
                        if ((a == 0 && b == 0 && c == 1) ||
                            (a == 1 && b == 0 && c == 0)) {
                            ok = 0;
                            break;
                        }
                    }
                    if (ok) {
                        transNext[base][cnt++] = q * 8 + r;
                    }
                }
                transCount[base] = cnt;
            }
        }
        trans_inited = 1;
    }

    /* Dynamic programming arrays */
    static seq_t dp[64];
    static seq_t next[64];
    memset(dp, 0, sizeof(dp));

    /* Initialise for the first two columns (any allowed pair) */
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            dp[i * 8 + j] = 1;

    /* Iterate columns 3 .. n */
    for (idx_t col = 3; col <= n; ++col) {
        /* zero next array */
        for (int i = 0; i < 64; ++i) next[i] = 0;

        for (int idx = 0; idx < 64; ++idx) {
            if (dp[idx] == 0) continue;
            int cnt = transCount[idx];
            for (int k = 0; k < cnt; ++k) {
                int nxt = transNext[idx][k];
                next[nxt] += dp[idx];
            }
        }
        /* swap dp and next for next iteration */
        for (int i = 0; i < 64; ++i) dp[i] = next[i];
    }

    /* Sum all ending states for total count */
    seq_t result = 0;
    for (int i = 0; i < 64; ++i) result += dp[i];
    return result;
}
