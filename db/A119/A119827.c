/*
 * A119827  Number of ternary words of length n with exactly one 000.
 * https://oeis.org/A119827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = number of ternary words of length n with exactly one occurrence of "000" */

/* Computes a(n) for any n >= 0.  Works for all supported seq_t sizes, including arbitrary‑precision. */
seq_t a(idx_t n)
{
    /* No words of length < 3 can contain "000" */
    if (n < 3)
        return (seq_t)0;

    /* dp[state][cnt]  state: 0 = no trailing zeros, 1 = trailing 0, 2 = trailing 00
       cnt : number of completed "000" patterns (0 or 1) encountered so far    */
    seq_t dp[3][2] = { {1,0}, {0,0}, {0,0} };   /* start at position 0 with state 0 and 0 patterns */

    for (idx_t pos = 0; pos < n; ++pos) {
        seq_t ndp[3][2] = { {0,0}, {0,0}, {0,0} };

        for (int state = 0; state < 3; ++state) {
            for (int cnt = 0; cnt < 2; ++cnt) {
                seq_t cur = dp[state][cnt];
                if (!cur) continue;

                /* Choose letter '1' or '2': reset state to 0, count unchanged */
                ndp[0][cnt] += cur;
                ndp[0][cnt] += cur;  /* twice for letters '1' and '2' */

                /* Choose letter '0' */
                int next_state;
                int next_cnt = cnt;
                if (state == 2) {
                    /* Completing a pattern */
                    if (cnt == 0) {          /* only allow if we haven't seen a pattern yet */
                        next_state = 2;
                        next_cnt = 1;
                    } else {
                        /* would create a second pattern – not allowed */
                        continue;
                    }
                } else if (state == 1) {
                    next_state = 2;          /* two trailing zeros now */
                } else { /* state == 0 */
                    next_state = 1;          /* one trailing zero */
                }
                ndp[next_state][next_cnt] += cur;
            }
        }

        /* Copy ndp back into dp for next iteration */
        dp[0][0] = ndp[0][0];  dp[0][1] = ndp[0][1];
        dp[1][0] = ndp[1][0];  dp[1][1] = ndp[1][1];
        dp[2][0] = ndp[2][0];  dp[2][1] = ndp[2][1];
    }

    /* Sum over all ending states with exactly one completed pattern */
    seq_t ans = dp[0][1] + dp[1][1] + dp[2][1];
    return ans;
}
