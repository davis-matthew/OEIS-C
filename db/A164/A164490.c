/*
 * A164490  Number of binary strings of length n with no substrings equal to
 *          0010 0011 or 0110.
 * https://oeis.org/A164490
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 24, u32 n <= 51, u64 n <= 104,
 *            u128 n <= 211, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* DP over last 3 bits; count binary strings of length n with no substrings
   equal to 0010, 0011, or 0110. */
seq_t a(idx_t n) {
    if (n < 4) return (seq_t)0;

    seq_t dp[8];
    seq_t next[8];
    /* initial strings of length 3: all 8 possible suffixes are allowed */
    for (int i = 0; i < 8; ++i) dp[i] = (seq_t)1;

    for (idx_t len = 4; len <= n; ++len) {
        /* zero next */
        for (int i = 0; i < 8; ++i) next[i] = 0;
        for (int s = 0; s < 8; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;
            /* try adding 0 and 1 */
            for (int b = 0; b <= 1; ++b) {
                int t = (s << 1) | b;          /* 4-bit window formed */
                if (t == 2 || t == 3 || t == 6) continue; /* forbidden */
                int ns = t & 7;                /* new suffix of length 3 */
                next[ns] += val;
            }
        }
        /* swap dp and next */
        for (int i = 0; i < 8; ++i) dp[i] = next[i];
    }

    /* sum all counts for the final length */
    seq_t total = 0;
    for (int i = 0; i < 8; ++i) total += dp[i];
    return total;
}
