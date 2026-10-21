/*
 * A207511  Number of nX5 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 1 1 1 vertically.
 * https://oeis.org/A207511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary matrices that avoid the patterns 0 0
 *            0 and 0 1 1 horizontally in each row and the patterns 0 0 1 and
 *            1 1 1 vertically in each column. Computed by a DP over allowed
 *            row states
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 26, u64 n <= 59, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


static const uint8_t allowed_rows[13] = {
    4, 5, 9, 10, 18, 20, 21, 25, 26, 28, 29, 30, 31
};

/* Transition mask: for each ordered pair (prev,cur) of allowed rows,
   gives a bitmask of allowed next rows that together with the pair
   satisfy the vertical restrictions. */
static uint16_t next_mask[13 * 13];
static int initialized = 0;

static void init_transitions(void)
{
    /* Pre‑compute which triples of rows are allowed. */
    for (int i = 0; i < 13; ++i) {
        for (int j = 0; j < 13; ++j) {
            uint16_t mask = 0;
            for (int k = 0; k < 13; ++k) {
                int ok = 1;
                for (int pos = 0; pos < 5 && ok; ++pos) {
                    int b0 = (allowed_rows[i] >> pos) & 1;
                    int b1 = (allowed_rows[j] >> pos) & 1;
                    int b2 = (allowed_rows[k] >> pos) & 1;
                    int triple = (b0 << 2) | (b1 << 1) | b2;
                    if (triple == 1 || triple == 7) /* 001 or 111 */
                        ok = 0;
                }
                if (ok) mask |= (1u << k);
            }
            next_mask[i * 13 + j] = mask;
        }
    }
    initialized = 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined, safe fallback */
    if (n == 1) return (seq_t)13;     /* single row: 13 possibilities */

    if (!initialized) init_transitions();

    static seq_t dp[13][13];
    static seq_t newdp[13][13];

    /* Initialisation for the first two rows: all pairs are allowed. */
    for (int i = 0; i < 13; ++i)
        for (int j = 0; j < 13; ++j)
            dp[i][j] = (seq_t)1;

    /* Extend the matrix row by row. */
    for (idx_t r = 3; r <= n; ++r) {
        for (int i = 0; i < 13; ++i)
            for (int j = 0; j < 13; ++j)
                newdp[i][j] = (seq_t)0;

        for (int prev = 0; prev < 13; ++prev) {
            for (int cur = 0; cur < 13; ++cur) {
                seq_t ways = dp[prev][cur];
                if (ways == 0) continue;
                uint16_t mask = next_mask[prev * 13 + cur];
                if (mask == 0) continue;
                for (int next = 0; next < 13; ++next) {
                    if (mask & (1u << next))
                        newdp[cur][next] += ways;
                }
            }
        }

        /* Move newdp into dp for the next iteration. */
        for (int i = 0; i < 13; ++i)
            for (int j = 0; j < 13; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* Sum over all ending pairs to obtain the total count. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 13; ++i)
        for (int j = 0; j < 13; ++j)
            total += dp[i][j];

    return total;
}
