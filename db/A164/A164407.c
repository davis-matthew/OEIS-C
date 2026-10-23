/*
 * A164407  Number of binary strings of length n with no substrings equal to
 *          0000 0001 or 0010.
 * https://oeis.org/A164407
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of binary strings of length n with no * substrings
 *            equal to 0000, 0001 or 0010. * Computed by a simple DP on the
 *            last up to 3 bits of the string
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 38, u64 n <= 78, u128 n <= 157,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 4; values for smaller n are
     * unspecified. Return 0 for those cases. */
    if (n < 4)
        return 0;

    /* dp[state] holds the number of strings of current length that end
     * with the 3‑bit pattern "state".  state is in [0,7]. */
    seq_t curr[8] = {0};
    seq_t next[8];
    curr[0] = 1;                 /* empty string */

    for (idx_t i = 0; i < n; ++i)    /* i is the current length before adding a bit */
    {
        /* clear next array */
        for (int j = 0; j < 8; ++j)
            next[j] = 0;

        for (int s = 0; s < 8; ++s)
        {
            seq_t val = curr[s];
            if (val == 0)
                continue;

            for (int b = 0; b < 2; ++b)
            {
                /* When length < 4 we do not have a full 4‑bit window,
                 * so no forbidden pattern check is necessary.  The
                 * check is performed only when i >= 3 (i.e., we are
                 * about to create the 4th bit). */
                if (i >= 3)
                {
                    unsigned pattern = ((s << 1) | b) & 0xF;
                    if (pattern <= 2)           /* patterns 0000, 0001, 0010 are forbidden */
                        continue;
                }

                int new_state = ((s << 1) | b) & 7;
                next[new_state] += val;
            }
        }

        /* move to next length */
        for (int j = 0; j < 8; ++j)
            curr[j] = next[j];
    }

    /* Sum over all ending states to obtain total number of valid strings. */
    seq_t sum = 0;
    for (int j = 0; j < 8; ++j)
        sum += curr[j];

    return sum;
}
