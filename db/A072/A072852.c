/*
 * A072852  Number of permutations satisfying i-2<=p(i)<=i+5, i=1..n.
 * https://oeis.org/A072852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Number of permutations p of {1..n} with i-2 <= p(i) <= i+5 for
 *            every i=1..n. Computation is performed by a dynamic‑programming
 *            state that tracks which of the 7 most recent values are already
 *            used (a sliding 7‑bit window; indices i-2,…,i+4). Complexity is
 *            O(n · 2^7) which is well below the limits
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* not defined for n=0, but harmless */

    /* Number of possible 7‑bit states */
    const int STATE_COUNT = 1 << 7;     /* 128 */
    seq_t dp[STATE_COUNT];
    seq_t dp_next[STATE_COUNT];

    /* initialise dp to zero */
    for (int s = 0; s < STATE_COUNT; ++s)
        dp[s] = 0;

    /* initial state for position 1:
       indices -1 and 0 (i-2, i-1) are considered already used
       bits 2..6 (indices 1..5) are free */
    int init_state = 0;
    init_state |= (1 << 0);   /* i-2 = -1 */
    init_state |= (1 << 1);   /* i-1 =  0 */
    dp[init_state] = 1;

    /* iterate positions 1..n */
    for (idx_t pos = 1; pos <= n; ++pos) {
        /* clear dp_next */
        for (int s = 0; s < STATE_COUNT; ++s)
            dp_next[s] = 0;

        /* compute the admissible j range for this position */
        int lower = (int)pos - 2;
        if (lower < 1) lower = 1;
        int upper = (int)pos + 5;
        if (upper > (int)n) upper = (int)n;

        /* iterate over all states */
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t cnt = dp[s];
            if (!cnt) continue;

            /* shift the window: drop the leftmost bit (i-2) */
            int shift = s >> 1;

            /* try every admissible j */
            for (int j = lower; j <= upper; ++j) {
                int new_state;
                if (j <= (int)pos + 4) {
                    int old_bit = j - ((int)pos - 2);  /* 0..6 */
                    if (s & (1 << old_bit)) continue; /* already used */

                    new_state = shift;
                    int new_bit = old_bit - 1;         /* may be -1 for j=i-2 */
                    if (new_bit >= 0)
                        new_state |= (1 << new_bit);
                    /* j = pos-2 leaves the window; no bit set */
                } else { /* j == pos+5 */
                    new_state = shift | (1 << 6);      /* new index becomes bit 6 */
                }
                dp_next[new_state] += cnt;
            }
        }

        /* move to next step */
        for (int s = 0; s < STATE_COUNT; ++s)
            dp[s] = dp_next[s];
    }

    /* sum over all final states */
    seq_t result = 0;
    for (int s = 0; s < STATE_COUNT; ++s)
        result += dp[s];
    return result;
}
