/*
 * A164421  Number of binary strings of length n with no substrings equal to
 *          0000 0010 or 1001.
 * https://oeis.org/A164421
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 40, u64 n <= 82, u128 n <= 165,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A164421: number of binary strings of length n avoiding
   the substrings 0000, 0010, and 1001.  The sequence is generated
   by a DP over the last up to three bits, yielding O(n) time. */

#include <stddef.h>

static const int offset[4] = {0, 1, 3, 7};

seq_t a(idx_t n)
{
    /* states: 0..14 encode (len, bits) where len = 0..3.
       offset[len] + bits is the state index.  The last 3 bits of the
       current string are stored in 'bits'. */
    seq_t dp[15] = {0};
    seq_t next[15];
    dp[0] = 1;  /* empty string */

    /* forbidden 4‑bit patterns: 0000 (0), 0010 (2), 1001 (9). */
    const unsigned forbidden[3] = {0, 2, 9};

    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 15; ++i) next[i] = 0;

        for (int s = 0; s < 15; ++s) {
            if (!dp[s]) continue;

            int len, bits;
            if (s == 0) { len = 0; bits = 0; }
            else if (s < 3) { len = 1; bits = s - 1; }
            else if (s < 7) { len = 2; bits = s - 3; }
            else { len = 3; bits = s - 7; }

            for (int b = 0; b <= 1; ++b) {
                int new_len = (len == 3) ? 3 : len + 1;
                int new_bits = ((bits << 1) | b) & ((1 << new_len) - 1);
                int new_s = offset[new_len] + new_bits;

                if (step >= 3) {            /* check newly formed 4‑bit suffix */
                    unsigned last4 = (bits << 1) | b;
                    if (last4 == forbidden[0] ||
                        last4 == forbidden[1] ||
                        last4 == forbidden[2])
                        continue;
                }
                next[new_s] += dp[s];
            }
        }

        /* copy next to dp for the next iteration */
        for (int i = 0; i < 15; ++i) dp[i] = next[i];
    }

    /* sum all states: any suffix of length ≤3 is allowed */
    seq_t total = 0;
    for (int i = 0; i < 15; ++i) total += dp[i];
    return total;
}
