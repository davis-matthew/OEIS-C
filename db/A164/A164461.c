/*
 * A164461  Number of binary strings of length n with no substrings equal to
 *          0001 0011 or 1110.
 * https://oeis.org/A164461
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 50, u64 n <= 103,
 *            u128 n <= 208, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation for A164461: number of binary strings of length n
 * with no substrings 0001, 0011, or 1110.
 * The recurrence is implemented via a 3‑bit suffix DP:
 *   dp[i][s] = number of length‑i strings ending with suffix s (s = 0..7).
 * Transition: append bit b ∈ {0,1}; new suffix s' = ((s << 1) | b) & 7.
 * If i+1 ≥ 4, the 4‑bit pattern ((s << 1) | b) must not be 1 (0001),
 * 3 (0011), or 14 (1110).  All other transitions are allowed.
 * The answer is the sum over all suffix states after n steps.
 */

seq_t a(idx_t n)
{
    if (n < 4) return 0;           /* values below the offset are irrelevant */

    /* DP arrays: 8 states for possible 3‑bit suffixes. */
    seq_t dp_cur[8] = { 0 };
    seq_t dp_next[8] = { 0 };

    dp_cur[0] = 1;                 /* start with the empty string, suffix 000 */

    for (idx_t i = 0; i < n; ++i) {
        /* reset next state array */
        for (int s = 0; s < 8; ++s) dp_next[s] = 0;

        for (int s = 0; s < 8; ++s) {
            if (dp_cur[s] == 0) continue;

            for (int b = 0; b < 2; ++b) {
                int new_state = ((s << 1) | b) & 7;
                if (i + 1 >= 4) {                    /* check 4‑bit substrings */
                    int pattern = (s << 1) | b;
                    if (pattern == 1 || pattern == 3 || pattern == 14)
                        continue;                 /* forbidden pattern */
                }
                dp_next[new_state] += dp_cur[s];
            }
        }
        /* copy dp_next to dp_cur for the next iteration */
        for (int s = 0; s < 8; ++s) dp_cur[s] = dp_next[s];
    }

    /* sum all suffix states: all counts represent distinct valid strings */
    seq_t total = 0;
    for (int s = 0; s < 8; ++s) total += dp_cur[s];
    return total;
}
