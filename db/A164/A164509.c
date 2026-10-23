/*
 * A164509  Number of binary strings of length n with no substrings equal to
 *          0101, 0110, or 1010.
 * https://oeis.org/A164509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) counts binary strings of length n * that do not contain the
 *            substrings 0101, 0110, or 1010. * * For n < 4 the values are 1,
 *            2, 4, 8 respectively. * For n ≥ 4 we use a 3‑bit state dynamic
 *            programming: * state = last three bits of the current string. *
 *            Transition from state s by appending bit...
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 40, u64 n <= 81, u128 n <= 163,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base values for n = 0 .. 3 */
    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 4;
    if (n == 3) return 8;

    /* dp[s] – number of strings of current length ending with suffix s (3 bits) */
    seq_t dp[8];
    seq_t next[8];

    /* initial length 3: each 3‑bit pattern occurs exactly once */
    for (int s = 0; s < 8; ++s)
        dp[s] = 1;

    /* iterate from length 4 up to n */
    for (idx_t len = 4; len <= n; ++len) {
        for (int s = 0; s < 8; ++s)
            next[s] = 0;

        for (int s = 0; s < 8; ++s) {
            seq_t cnt = dp[s];
            if (cnt == 0) continue;

            /* append 0 */
            int new4 = (s << 1);          /* 4-bit value ending with 0 */
            if (new4 != 5 && new4 != 6 && new4 != 10) {
                int new_state = new4 & 0x7;
                next[new_state] += cnt;
            }

            /* append 1 */
            new4 = (s << 1) | 1;          /* 4-bit value ending with 1 */
            if (new4 != 5 && new4 != 6 && new4 != 10) {
                int new_state = new4 & 0x7;
                next[new_state] += cnt;
            }
        }

        /* copy next to dp for the next iteration */
        for (int s = 0; s < 8; ++s)
            dp[s] = next[s];
    }

    /* total count is the sum over all states */
    seq_t total = 0;
    for (int s = 0; s < 8; ++s)
        total += dp[s];
    return total;
}
