/*
 * A183315  Number of n X 3 binary arrays with an element zero only if there
 *          are an even number of ones to its left and an even number of ones
 *          above it.
 * https://oeis.org/A183315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* DP over column‑parity states (3 bits) to count n×3 binary arrays
   where a zero is allowed only if the number of ones to its left
   and above it are both even. */

seq_t a(idx_t n)
{
    /* Transition table: trans[prev_state][next_state] = number of
       valid 3‑bit rows turning prev_state into next_state.  The table
       is constructed once on the first call. */
    static unsigned char trans[8][8] = {0};
    static int initialized = 0;

    if (!initialized) {
        for (int st = 0; st < 8; ++st) {
            int c1 = st & 1;
            int c2 = (st >> 1) & 1;
            int c3 = (st >> 2) & 1;
            for (int pat = 0; pat < 8; ++pat) {
                int b1 = pat & 1;
                int b2 = (pat >> 1) & 1;
                int b3 = (pat >> 2) & 1;
                int ok = 1;

                /* Column 1 */
                if (b1 == 0 && c1 != 0) ok = 0;
                /* Column 2 */
                if (b2 == 0) {
                    if (c2 != 0) ok = 0;
                    if (b1 != 0) ok = 0;
                }
                /* Column 3 */
                if (b3 == 0) {
                    if (c3 != 0) ok = 0;
                    if ((b1 ^ b2) != 0) ok = 0;
                }

                if (ok) {
                    int new_st = st ^ pat;   /* flip bits where bj == 1 */
                    trans[st][new_st]++;     /* count this pattern */
                }
            }
        }
        initialized = 1;
    }

    /* Dynamic programming over rows.  dp_prev[s] holds the number of
       ways to reach state s after processing some number of rows. */
    seq_t dp_prev[8] = {0}, dp_next[8];
    dp_prev[0] = (seq_t)1;                /* start with zero parities */

    for (idx_t row = 1; row <= n; ++row) {
        for (int j = 0; j < 8; ++j) dp_next[j] = 0;
        for (int s1 = 0; s1 < 8; ++s1) {
            seq_t val = dp_prev[s1];
            if (val == 0) continue;
            for (int s2 = 0; s2 < 8; ++s2) {
                unsigned char m = trans[s1][s2];
                if (m) dp_next[s2] += val * (seq_t)m;
            }
        }
        for (int j = 0; j < 8; ++j) dp_prev[j] = dp_next[j];
    }

    /* Sum over all final states: the column parities after the last
       row are irrelevant to the constraints. */
    seq_t total = 0;
    for (int j = 0; j < 8; ++j) total += dp_prev[j];
    return total;
}
