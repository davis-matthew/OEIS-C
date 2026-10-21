/*
 * A207770  Number of n X 4 0..1 arrays avoiding 0 0 0 and 1 1 1 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A207770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 18, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The number of n×4 binary arrays that avoid 000 and 111 in every row
   (horizontal rule) and avoid 001 and 010 in every column of any 3
   consecutive rows (vertical rule).  This function builds the arrays
   row‑by‑row using dynamic programming on the last two rows. */
seq_t a(idx_t n)
{
    /* Precompute all 4‑bit row patterns that do not contain 000 or 111. */
    static int init = 0;
    static uint8_t rows[16];
    static uint8_t nrows = 0;
    static seq_t dp[256];
    static seq_t dp_next[256];

    if (!init) {
        nrows = 0;
        for (int r = 0; r < 16; ++r) {
            int b0 = (r >> 0) & 1;
            int b1 = (r >> 1) & 1;
            int b2 = (r >> 2) & 1;
            int b3 = (r >> 3) & 1;

            /* horizontal rule: no 000 or 111 in any triple of bits */
            int ok = 1;
            if ((b0==0 && b1==0 && b2==0) ||
                (b1==0 && b2==0 && b3==0) ||
                (b0==1 && b1==1 && b2==1) ||
                (b1==1 && b2==1 && b3==1))
                ok = 0;

            if (ok) {
                rows[nrows++] = (uint8_t)r;
            }
        }
        init = 1;
    }

    if (n == 0) return 0;            /* out of range, unused */
    if (n == 1) return (seq_t)nrows;  /* one row, no vertical restriction */
    if (n == 2) return (seq_t)nrows * (seq_t)nrows;  /* two rows, no vertical rule */

    /* Initialize dp for the first two rows */
    for (int i = 0; i < 256; ++i) dp[i] = 0;

    for (int i = 0; i < nrows; ++i) {
        uint8_t r1 = rows[i];
        for (int j = 0; j < nrows; ++j) {
            uint8_t r2 = rows[j];
            dp[(r1 << 4) | r2] += 1;
        }
    }

    /* DP for rows 3 … n */
    for (idx_t row = 3; row <= n; ++row) {
        for (int i = 0; i < 256; ++i) dp_next[i] = 0;

        for (int s = 0; s < 256; ++s) {
            seq_t ways = dp[s];
            if (ways == 0) continue;

            uint8_t prevprev = s >> 4;
            uint8_t prev      = s & 0xF;

            for (int k = 0; k < nrows; ++k) {
                uint8_t curr = rows[k];

                /* vertical rule: for each of 4 columns, the triple
                   (prevprev, prev, curr) must not be 0‑0‑1 or 0‑1‑0 */
                int bad = 0;
                for (int j = 0; j < 4; ++j) {
                    uint8_t a = (prevprev >> j) & 1;
                    uint8_t b = (prev      >> j) & 1;
                    uint8_t c = (curr      >> j) & 1;
                    if ((a==0 && b==0 && c==1) || (a==0 && b==1 && c==0)) {
                        bad = 1;
                        break;
                    }
                }
                if (!bad) {
                    int idx = (prev << 4) | curr;
                    dp_next[idx] += ways;
                }
            }
        }

        /* move next state into current */
        for (int i = 0; i < 256; ++i) dp[i] = dp_next[i];
    }

    /* Sum all final states */
    seq_t total = 0;
    for (int i = 0; i < 256; ++i) total += dp[i];
    return total;
}
