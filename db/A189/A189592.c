/*
 * A189592  Number of permutations of 1..n with displacements restricted to
 *          {-5,-4,0,1,2,3}.
 * https://oeis.org/A189592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of permutations of 1..n whose displacements belong
 *            to the set {-5,-4,0,1,2,3}. This is computed by dynamic
 *            programming over a sliding window of 9 columns. The state mask
 *            keeps track of which of the columns i-5 .. i+3 are already used
 *            by earlier rows. After processing all rows the lo...
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 49, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* window width = max negative displacement (5) + max positive (3) + 1 = 9 */
    const int W = 9;
    const int MASK_TOT = 1 << W;        /* 512 states */
    const int LOW5 = (1 << 5) - 1;      /* bits 0..4 correspond to columns <1 before first row */

    /* bit positions corresponding to allowed displacements
       (offset = bit - 5  ->  -5,-4,0,1,2,3) */
    static const int REL[6] = {0, 1, 5, 6, 7, 8};

    seq_t dp_cur[MASK_TOT];
    seq_t dp_next[MASK_TOT];

    /* initialise dp_cur to zero */
    for (int m = 0; m < MASK_TOT; ++m) dp_cur[m] = 0;
    /* initial mask: columns < 1 are already occupied */
    int init_mask = LOW5;
    dp_cur[init_mask] = 1;

    for (idx_t i = 1; i <= n; ++i) {
        /* reset dp_next */
        for (int m = 0; m < MASK_TOT; ++m) dp_next[m] = 0;

        for (int mask = 0; mask < MASK_TOT; ++mask) {
            seq_t ways = dp_cur[mask];
            if (!ways) continue;

            for (int k = 0; k < 6; ++k) {
                int rel = REL[k];
                int64_t col = (int64_t)i + (int64_t)(rel - 5);
                if (col < 1 || col > (int64_t)n) continue;   /* out of bounds */
                if (mask & (1 << rel)) continue;             /* already used */

                int new_mask = ((mask | (1 << rel)) >> 1);
                dp_next[new_mask] += ways;
            }
        }
        /* copy dp_next into dp_cur for next row */
        for (int m = 0; m < MASK_TOT; ++m) dp_cur[m] = dp_next[m];
    }

    /* sum over states where the 5 lowest bits (columns 1..5) are filled */
    seq_t result = 0;
    for (int mask = 0; mask < MASK_TOT; ++mask) {
        if ((mask & LOW5) == LOW5) {
            result += dp_cur[mask];
        }
    }
    return result;
}
