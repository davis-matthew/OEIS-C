/*
 * A202909  Number of nX1 0..7 arrays with every nonzero element less than or
 *          equal to some horizontal or vertical neighbor.
 * https://oeis.org/A202909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A202909.
   Count of n×1 arrays of digits 0..7 in which every non‑zero entry
   is ≤ a horizontal or vertical neighbour.
   The DP below builds the sequence left‑to‑right, keeping the last
   two entries.  An entry that is strictly larger than its left
   neighbour must have a following entry ≥ it; this is enforced
   on the transition.
   At the end we exclude states where the last entry is a strict
   left‑peak (no right neighbour).  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety, though n≥1 for the real sequence */

    /* 8 x 8 DP tables: dp[left][cur] = number of ways to reach state
       where the two most recent entries are (left, cur). */
    seq_t dp[8][8]  = {0};
    seq_t dp_next[8][8] = {0};

    /* position 1: left is dummy 0, cur can be anything 0..7 */
    for (idx_t cur = 0; cur < 8; ++cur)
        dp[0][cur] = 1;

    /* iterate positions 2..n */
    for (idx_t pos = 2; pos <= n; ++pos)
    {
        /* clear dp_next */
        for (idx_t i = 0; i < 8; ++i)
            for (idx_t j = 0; j < 8; ++j)
                dp_next[i][j] = 0;

        for (idx_t left = 0; left < 8; ++left)
        {
            for (idx_t cur = 0; cur < 8; ++cur)
            {
                seq_t cnt = dp[left][cur];
                if (cnt == 0) continue;

                /* If cur>0 and left<cur, cur must be <= next entry. */
                int need_high = (cur > 0 && left < cur);

                for (idx_t nxt = 0; nxt < 8; ++nxt)
                {
                    if (need_high && nxt < cur) continue;
                    dp_next[cur][nxt] += cnt;
                }
            }
        }

        /* swap dp and dp_next */
        for (idx_t i = 0; i < 8; ++i)
            for (idx_t j = 0; j < 8; ++j)
                dp[i][j] = dp_next[i][j];
    }

    /* sum over final states, excluding last entry that is a strict left‑peak */
    seq_t result = 0;
    for (idx_t left = 0; left < 8; ++left)
        for (idx_t cur = 0; cur < 8; ++cur)
        {
            seq_t cnt = dp[left][cur];
            if (cnt == 0) continue;
            if (cur > 0 && left < cur) continue;   /* last entry invalid */
            result += cnt;
        }

    return result;
}
