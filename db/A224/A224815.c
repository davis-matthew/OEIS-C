/*
 * A224815  Number of subsets of {1,2,...,n-8} without differences equal to 4
 *          or 8.
 * https://oeis.org/A224815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 34, u32 n <= 63, u64 n <= 121,
 *            u128 n <= 237, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A224815 – Number of subsets of {1,…,n‑8} with no two
   elements differing by 4 or 8.  The function uses a sliding‑window
   DP over the last eight positions.  For each position i (1 ≤ i ≤ n‑8)
   we keep a 8‑bit mask that records whether the previous eight
   positions (i‑1,…,i‑8) were chosen.  When processing position i we
   may choose it only if the bits corresponding to i‑4 (bit 3) and
   i‑8 (bit 7) are zero.  The DP runs in O(n·256) time and uses no
   dynamic allocation. */
seq_t a(idx_t n)
{
    /* For n < 8 the set {1,…,n‑8} is empty, thus only the empty
       subset is allowed. */
    if (n < 8)
        return (seq_t)1;

    idx_t m = n - 8;          /* number of elements to consider */

    seq_t dp[256]   = {0};
    seq_t next[256] = {0};

    dp[0] = (seq_t)1;         /* no positions processed yet */

    for (idx_t pos = 0; pos < m; ++pos) {
        /* reset next array */
        for (int i = 0; i < 256; ++i)
            next[i] = (seq_t)0;

        for (int mask = 0; mask < 256; ++mask) {
            seq_t ways = dp[mask];
            if (!ways)
                continue;

            /* Case 1: do NOT take current element */
            int newmask = (mask << 1) & 0xFF;      /* shift window */
            next[newmask] = next[newmask] + ways;

            /* Case 2: take current element – only if i‑4 and i‑8
               are not already selected. */
            if (((mask >> 3) & 1) == 0 && ((mask >> 7) & 1) == 0) {
                int newmask2 = ((mask << 1) | 1) & 0xFF;
                next[newmask2] = next[newmask2] + ways;
            }
        }

        /* move to next position */
        for (int i = 0; i < 256; ++i)
            dp[i] = next[i];
    }

    /* sum all counts – final result does not depend on trailing mask */
    seq_t result = (seq_t)0;
    for (int i = 0; i < 256; ++i)
        result = result + dp[i];

    return result;
}
