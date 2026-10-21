/*
 * A094287  Number of (s(0), s(1), ..., s(n)) such that 0 < s(i) < 7 and |s(i)
 *          - s(i-1)| <= 1 for i = 1,2,...,n, s(0) = 1, s(n) = 1.
 * https://oeis.org/A094287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A094287.
 * This is the number of length‑n walks on the path graph with
 * 6 vertices (labels 1…6) that start and finish at vertex 1.
 * Let h = s(i)-1 (0…5).  From any state h we may
 * stay (h), step up (h+1) or step down (h-1) provided the
 * destination remains in 0…5.
 *
 * Recurrence:
 *   dp[0][0] = 1
 *   dp[t+1][h] = dp[t][h]                   (stay)
 *             + dp[t][h-1]   (if h>0)       (step up)
 *             + dp[t][h+1]   (if h<5)       (step down)
 *   a(n) = dp[n][0]
 *
 * Complexity: O(n) time, O(1) memory.
 * Works for any of the supported seq_t / idx_t integer types.
 */

seq_t a(idx_t n)
{
    /* dp holds the counts for the current step.  States 0…5. */
    seq_t dp[6] = {0};
    dp[0] = 1;               /* start at node 1 → height 0 */

    for (idx_t step = 1; step <= n; ++step)
    {
        seq_t ndp[6] = {0};

        for (idx_t h = 0; h < 6; ++h)
        {
            seq_t val = dp[h];
            if (!val) continue;

            /* stay */
            ndp[h] += val;

            /* step down → h-1 */
            if (h > 0)        ndp[h-1] += val;

            /* step up → h+1 */
            if (h < 5u)      ndp[h+1] += val;
        }

        /* copy next step into current */
        for (idx_t h = 0; h < 6; ++h)
            dp[h] = ndp[h];
    }

    return dp[0];   /* end at height 0 (s(n)=1) */
}
