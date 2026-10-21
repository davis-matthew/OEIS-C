/*
 * A207414  Number of nX3 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A207414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of n×3 binary arrays avoiding horizontal patterns 001 and
 *            010, * and vertical patterns 011 and 101. Ex: a(1)=6, a(2)=36. *
 *            The function implements a transfer‑matrix DP over 3‑bit row
 *            states
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 22, u64 n <= 48, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Check a vertical triple in a single column. */
static bool valid_vertical(uint8_t b1, uint8_t b2, uint8_t b3)
{
    return !(b1==0 && b2==1 && b3==1) &&
           !(b1==1 && b2==0 && b3==1);
}

/* Check that the three rows (prev_prev, prev, cur) satisfy vertical
 * constraints for all three columns. */
static bool valid_transition(uint8_t pp, uint8_t p, uint8_t cur)
{
    if (!valid_vertical((pp>>0)&1, (p>>0)&1, (cur>>0)&1)) return false;
    if (!valid_vertical((pp>>1)&1, (p>>1)&1, (cur>>1)&1)) return false;
    if (!valid_vertical((pp>>2)&1, (p>>2)&1, (cur>>2)&1)) return false;
    return true;
}

/* 3‑bit rows that contain no horizontal 001 or 010. */
static const uint8_t allowed_rows[6] = {0, 3, 4, 5, 6, 7};

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* out of defined range */
    if (n == 1) return (seq_t)6;       /* only horizontal constraints */

    seq_t dp[8][8] = {0};              /* counts for pair (prev_prev, prev) */
    seq_t ndp[8][8] = {0};

    /* Initialise for the first two rows: any pair of allowed rows. */
    for (size_t i = 0; i < 6; ++i) {
        uint8_t r1 = allowed_rows[i];
        for (size_t j = 0; j < 6; ++j) {
            uint8_t r2 = allowed_rows[j];
            dp[r1][r2] = 1;
        }
    }

    /* Process rows 3 .. n. */
    if (n >= 3) {
        for (idx_t row = 3; row <= n; ++row) {
            /* clear ndp */
            for (size_t i = 0; i < 8; ++i)
                for (size_t j = 0; j < 8; ++j)
                    ndp[i][j] = 0;

            for (size_t i = 0; i < 6; ++i) {
                uint8_t pp = allowed_rows[i];
                for (size_t j = 0; j < 6; ++j) {
                    uint8_t p = allowed_rows[j];
                    seq_t ways = dp[pp][p];
                    if (!ways) continue;
                    for (size_t k = 0; k < 6; ++k) {
                        uint8_t c = allowed_rows[k];
                        if (valid_transition(pp, p, c))
                            ndp[p][c] += ways;
                    }
                }
            }
            /* copy ndp back to dp */
            for (size_t i = 0; i < 8; ++i)
                for (size_t j = 0; j < 8; ++j)
                    dp[i][j] = ndp[i][j];
        }
    }

    /* Sum over all ending pairs of rows. */
    seq_t total = 0;
    for (size_t i = 0; i < 6; ++i) {
        uint8_t p = allowed_rows[i];
        for (size_t j = 0; j < 6; ++j) {
            uint8_t c = allowed_rows[j];
            total += dp[p][c];
        }
    }
    return total;
}
