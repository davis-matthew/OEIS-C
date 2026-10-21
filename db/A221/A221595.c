/*
 * A221595  Number of 0..7 arrays of length n with each element differing from
 *          at least one neighbor by 1 or less.
 * https://oeis.org/A221595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: dynamic programming over the last value and a
   flag indicating whether that value is already satisfied by
   its left neighbour.  Returns the number of 0..7 arrays of
   length n where every element has a neighbour whose value
   differs by at most 1.  No direct recurrence is used. */
seq_t a(idx_t n)
{
    if (n == 0)          /* not expected by OEIS offset, but safe */
        return (seq_t)0;
    if (n == 1)          /* no neighbours → no array satisfies the rule */
        return (seq_t)0;

    /* dp[val][sat]  –  number of sequences ending with value 'val'
       where 'sat' == 1 means that element is already satisfied by
       its left neighbour.  */
    seq_t dp[8][2];
    seq_t next[8][2];

    /* initialise for position 0 (first element) – it has no left neighbour */
    for (idx_t v = 0; v < 8; ++v) {
        dp[v][0] = (seq_t)1;
        dp[v][1] = (seq_t)0;
    }

    /* build sequences of length 2 … n */
    for (idx_t pos = 1; pos < n; ++pos) {
        /* clear next */
        for (idx_t v = 0; v < 8; ++v) {
            next[v][0] = (seq_t)0;
            next[v][1] = (seq_t)0;
        }

        for (idx_t prev = 0; prev < 8; ++prev) {
            for (int sat = 0; sat <= 1; ++sat) {
                seq_t cur = dp[prev][sat];
                if (cur == (seq_t)0)
                    continue;

                for (idx_t nxt = 0; nxt < 8; ++nxt) {
                    unsigned int d = (prev > nxt) ? (unsigned int)(prev - nxt)
                                                  : (unsigned int)(nxt - prev);
                    if (sat == 0 && d > 1)        /* prev not yet satisfied */
                        continue;

                    int new_sat = (d <= 1) ? 1 : 0;
                    next[nxt][new_sat] += cur;
                }
            }
        }

        /* swap dp and next for the next iteration */
        for (idx_t v = 0; v < 8; ++v) {
            dp[v][0] = next[v][0];
            dp[v][1] = next[v][1];
        }
    }

    /* last element must be satisfied by its left neighbour */
    seq_t ans = (seq_t)0;
    for (idx_t v = 0; v < 8; ++v)
        ans += dp[v][1];

    return ans;
}
