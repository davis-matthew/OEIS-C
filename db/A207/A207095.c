/*
 * A207095  Number of 0..3 arrays x(0..n-1) of n elements with each no smaller
 *          than the sum of its two previous neighbors modulo 4.
 * https://oeis.org/A207095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A207095
   Number of 0..3 arrays x(0..n-1) with each element no smaller
   than the sum of its two previous neighbors modulo 4.
   The recurrence is implemented as a simple dynamic programming
   over the last two chosen values.
*/

seq_t a(idx_t n)
{
    /* dp[(p<<2)|q] holds the number of sequences ending with
       previous two values p,q (0 <= p,q < 4).
       Start with the imaginary state (0,0).                */
    seq_t dp[16]      = {0};
    seq_t next[16]    = {0};

    dp[0] = 1;                 /* state (0,0) */

    for (idx_t step = 0; step < n; ++step)
    {
        /* clear next state */
        for (int i = 0; i < 16; ++i) next[i] = 0;

        /* process all current states */
        for (int p = 0; p < 4; ++p)
        {
            for (int q = 0; q < 4; ++q)
            {
                seq_t c = dp[(p << 2) | q];
                if (!c) continue;

                int sum_mod = (p + q) & 3;  /* (p+q) mod 4 */

                /* choose new element r satisfying r >= sum_mod */
                for (int r = sum_mod; r < 4; ++r)
                {
                    next[(q << 2) | r] += c;
                }
            }
        }

        /* move to next step */
        for (int i = 0; i < 16; ++i) dp[i] = next[i];
    }

    /* total number of valid sequences of length n */
    seq_t total = 0;
    for (int i = 0; i < 16; ++i) total += dp[i];
    return total;
}
