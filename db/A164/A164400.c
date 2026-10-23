/*
 * A164400  Number of binary strings of length n with no substrings equal to
 *          0001 or 1100.
 * https://oeis.org/A164400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of binary strings of length n that avoid the
 *            substrings 0001 and 1100. The following implementation uses a
 *            deterministic automaton over the last three bits; it runs in
 *            O(n) time with a constant space of 8 states
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 40, u64 n <= 80, u128 n <= 162,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* dp[mask] holds the number of length- current strings whose
       last three bits are represented by the 3‑bit mask 'mask'.
       The mask is an integer 0…7, where the binary form of the mask
       is the last up to three bits (for very short strings the
       leading bits are unused).  The automaton forbids any transition
       that would create the pattern 0001 or 1100 once the current
       length is at least 3. */
    seq_t dp[8];
    for (int i = 0; i < 8; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;                           /* empty string */

    for (idx_t len = 0; len < n; ++len)
    {
        seq_t newdp[8] = {(seq_t)0,(seq_t)0,(seq_t)0,(seq_t)0,
                          (seq_t)0,(seq_t)0,(seq_t)0,(seq_t)0};

        for (int mask = 0; mask < 8; ++mask)
        {
            const seq_t c = dp[mask];
            /* adding a 0 or a 1 */
            for (int bit = 0; bit < 2; ++bit)
            {
                if (len >= 3)                     /* pattern check */
                {
                    int pat = (mask << 1) | bit;  /* 4‑bit pattern */
                    if (pat == 1 || pat == 12)    /* 0001 or 1100 */
                        continue;
                }
                int newmask = ((mask << 1) | bit) & 7;
                newdp[newmask] += c;
            }
        }
        for (int i = 0; i < 8; ++i) dp[i] = newdp[i];
    }

    seq_t sum = (seq_t)0;
    for (int i = 0; i < 8; ++i) sum += dp[i];
    return sum;
}
