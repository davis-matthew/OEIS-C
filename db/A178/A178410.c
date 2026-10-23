/*
 * A178410  Number of ways to place 8 nonattacking wazirs on an 8 X n board.
 * https://oeis.org/A178410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1048576*n^8 -17432576*n^7 +136349696*n^6 -658958720*n^5
 *            +2161896569*n^4 -4945969574*n^3 +7719028159*n^2 -7516702410*n
 *            +3494080800) / 2520, n >= 7 For n < 7 a(n) is computed by a
 *            small DP over 8×n grids
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 9, u64 n <= 122, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the trivial case for n == 0 (not used by OEIS) */
    if (n == 0)
        return 0;

    /* For n < 7 use a brute‑force DP on the 8×n board */
    if (n < 7) {
        const int R = 8;
        const int MAXMASK = 1 << R;   /* 256 */

        /* static data, computed once */
        static int initialized = 0;
        static int valid_masks[256];
        static int mask_pop[256];
        static int num_valid = 0;

        if (!initialized) {
            for (int mask = 0; mask < MAXMASK; ++mask) {
                if ((mask & (mask << 1)) != 0)          /* vertical adjacency */
                    continue;
                valid_masks[num_valid] = mask;
                /* popcount */
                int pc = 0, tmp = mask;
                while (tmp) { tmp &= tmp - 1; ++pc; }
                mask_pop[num_valid] = pc;
                ++num_valid;
            }
            initialized = 1;
        }

        /* DP arrays: dp[mask][k] – number of ways to reach this column
           with 'mask' occupied in the current column and 'k' wazirs in total */
        seq_t dp_prev[256][9];
        seq_t dp_curr[256][9];
        /* zero‑initialize */
        for (int i = 0; i < 256; ++i)
            for (int k = 0; k < 9; ++k)
                dp_prev[i][k] = 0;
        dp_prev[0][0] = 1;          /* before first column */

        for (uint64_t col = 0; col < n; ++col) {
            /* zero dp_curr */
            for (int i = 0; i < 256; ++i)
                for (int k = 0; k < 9; ++k)
                    dp_curr[i][k] = 0;

            /* transition */
            for (int i = 0; i < num_valid; ++i) {
                int prev_mask = valid_masks[i];
                int pc_prev   = mask_pop[i];
                for (int k = 0; k <= 8; ++k) {
                    seq_t val = dp_prev[prev_mask][k];
                    if (val == 0)
                        continue;
                    for (int j = 0; j < num_valid; ++j) {
                        int curr_mask = valid_masks[j];
                        if (prev_mask & curr_mask)          /* horizontal conflict */
                            continue;
                        int newk = k + mask_pop[j];
                        if (newk <= 8)
                            dp_curr[curr_mask][newk] += val;
                    }
                }
            }

            /* swap dp_prev and dp_curr (by copying) */
            for (int i = 0; i < 256; ++i)
                for (int k = 0; k < 9; ++k)
                    dp_prev[i][k] = dp_curr[i][k];
        }

        /* sum over all final masks with exactly 8 wazirs */
        seq_t result = 0;
        for (int mask = 0; mask < 256; ++mask)
            result += dp_prev[mask][8];
        return result;
    }

    /* For n >= 7 use the polynomial formula */
    static const long long coeff[9] = {
        1048576LL,          /* n^8  */
        -17432576LL,       /* n^7  */
        136349696LL,       /* n^6  */
        -658958720LL,      /* n^5  */
        2161896569LL,      /* n^4  */
        -4945969574LL,     /* n^3  */
        7719028159LL,      /* n^2  */
        -7516702410LL,     /* n^1  */
        3494080800LL       /* n^0  */
    };
    __int128 val = coeff[0];
    for (int i = 1; i < 9; ++i) {
        val = val * (__int128)n + (__int128)coeff[i];
    }
    val /= 2520;
    return (seq_t)val;
}
