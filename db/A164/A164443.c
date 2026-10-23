/*
 * A164443  Number of binary strings of length n with no substrings equal to
 *          0000, 0111 or 1110.
 * https://oeis.org/A164443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 39, u64 n <= 80, u128 n <= 160,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 2.  For n ≥ 2 the sequence is obtained by
   iterating over an automaton with 8 states that encode the last run of
   bits.  Each state transition adds one bit to the string while
   preserving the forbidden patterns 0000, 0111, and 1110.  The sum of
   state counts after n bits equals a(n).  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    /* Dyck DP arrays for the 8 automaton states:
       0: zero run length 1          (Z1)
       1: zero run length 2          (Z2)
       2: zero run length 3          (Z3)
       3: one run length 1 after 0   (O1_az)
       4: one run length 2 after 0   (O2_az)
       5: one run length 1 not after0(O1_naz)
       6: one run length 2 not after0(O2_naz)
       7: one run length ≥3 not after0(O3_p)   */
    seq_t dp[8] = {0};
    /* initial step: first bit is 0 or 1 */
    dp[0] = 1;   /* start with 0 */
    dp[5] = 1;   /* start with 1 */

    for (idx_t i = 1; i < n; ++i) {
        seq_t ndp[8] = {0};

        /* from state 0 (Z1) */
        ndp[1] += dp[0];   /* add 0 -> Z2 */
        ndp[3] += dp[0];   /* add 1 -> O1_az */

        /* from state 1 (Z2) */
        ndp[2] += dp[1];   /* add 0 -> Z3 */
        ndp[3] += dp[1];   /* add 1 -> O1_az */

        /* from state 2 (Z3) */
        ndp[3] += dp[2];   /* add 1 -> O1_az */
        /* add 0 would go to dead - ignore */

        /* from state 3 (O1_az) */
        ndp[4] += dp[3];   /* add 1 -> O2_az */
        ndp[0] += dp[3];   /* add 0 -> Z1 */

        /* from state 4 (O2_az) */
        ndp[0] += dp[4];   /* add 0 -> Z1 */
        /* add 1 would go to dead - ignore */

        /* from state 5 (O1_naz) */
        ndp[6] += dp[5];   /* add 1 -> O2_naz */
        ndp[0] += dp[5];   /* add 0 -> Z1 */

        /* from state 6 (O2_naz) */
        ndp[7] += dp[6];   /* add 1 -> O3_p */
        ndp[0] += dp[6];   /* add 0 -> Z1 */

        /* from state 7 (O3_p) */
        ndp[7] += dp[7];   /* add 1 -> O3_p */
        /* add 0 would go to dead - ignore */

        /* copy back */
        for (int s = 0; s < 8; ++s)
            dp[s] = ndp[s];
    }

    /* sum over all non-dead states to obtain a(n) */
    seq_t total = 0;
    for (int s = 0; s < 8; ++s)
        total += dp[s];
    return total;
}
