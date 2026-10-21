/*
 * A073981  Number of strings of length n over Z_6 with trace 1 and subtrace
 *          4.
 * https://oeis.org/A073981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A073981 – Number of strings of length n over Z₆ with trace 1 and subtrace 4.
   The algorithm uses a dynamic‑programming recurrence over trace and subtrace modulo 6.
   Let dp[t][s] be the number of length‑k strings whose trace ≡ t (mod 6) and
   subtrace ≡ s (mod 6).  For k ≥ 2 the transition is
        dp_next[(t + x) % 6][(s + x * t) % 6] += dp[t][s]   for all x = 0…5.
   Initialisation for length 1: dp[d][0] = 1 for d = 0…5 (the only subtrace is 0).
   The required term is dp[1][4] after n steps.      */

seq_t a(idx_t n)
{
    static seq_t dp[6][6];
    static seq_t ndp[6][6];

    /* initialise dp for strings of length 1 */
    for (idx_t t = 0; t < 6; ++t)
        for (idx_t s = 0; s < 6; ++s)
            dp[t][s] = 0;
    for (idx_t d = 0; d < 6; ++d)
        dp[d][0] = 1;

    if (n == 1)
        return dp[1][4];          /* returns 0 – impossible for length 1 */

    /* iterate from length 2 up to n */
    for (idx_t step = 2; step <= n; ++step)
    {
        /* clear ndp */
        for (idx_t t = 0; t < 6; ++t)
            for (idx_t s = 0; s < 6; ++s)
                ndp[t][s] = 0;

        /* transition */
        for (idx_t t = 0; t < 6; ++t)
        {
            for (idx_t s = 0; s < 6; ++s)
            {
                seq_t val = dp[t][s];
                if (val == 0)
                    continue;
                for (idx_t x = 0; x < 6; ++x)
                {
                    idx_t tn = (t + x) % 6;
                    /* (x * t) fits in a small integer */
                    idx_t subadd = (x * t) % 6;
                    idx_t sn = (s + subadd) % 6;
                    ndp[tn][sn] += val;
                }
            }
        }

        /* copy ndp back to dp for the next iteration */
        for (idx_t t = 0; t < 6; ++t)
            for (idx_t s = 0; s < 6; ++s)
                dp[t][s] = ndp[t][s];
    }

    return dp[1][4];
}
