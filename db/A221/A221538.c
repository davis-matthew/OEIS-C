/*
 * A221538  Number of 0..4 arrays of length n with each element differing from
 *          at least one neighbor by something other than 1, starting with 0.
 * https://oeis.org/A221538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A221538: Number of 0..4 arrays of length n with each element
   differing from at least one neighbor by something other than 1,
   starting with 0.  The sequence satisfies the recurrence
   a(1)=0, a(2)=4, and for n>1 a(n)=sum_{v=-,w=,need} ... (see code). */

seq_t a(idx_t n)
{
    /* DP state: dp[last_value][need_flag]
       last_value ∈ {0..4}, need_flag ∈ {0,1} */
    seq_t dp[5][2] = {0};
    seq_t next[5][2] = {0};

    /* Initial state for n=1.  The first element is 0 and
       needs a right neighbor to satisfy the condition. */
    dp[0][1] = 1;

    /* Iterate positions 1..n-1 (add one element each step). */
    for (idx_t len = 1; len < n; ++len) {
        /* reset next array */
        for (int v = 0; v < 5; ++v)
            for (int f = 0; f < 2; ++f)
                next[v][f] = 0;

        /* transition */
        for (int v = 0; v < 5; ++v) {
            for (int need = 0; need <= 1; ++need) {
                seq_t cur = dp[v][need];
                if (!cur) continue;
                for (int w = 0; w < 5; ++w) {
                    int diff = v - w;
                    if (diff < 0) diff = -diff;

                    /* If the previous element still needs a satisfying
                       neighbor (need==1), the new element must NOT have
                       difference 1 with it. */
                    if (need == 1 && diff == 1)
                        continue;

                    /* For the new element w, check if its condition
                       is already satisfied by its left neighbor. */
                    int need2 = (diff == 1) ? 1 : 0;

                    next[w][need2] += cur;
                }
            }
        }

        /* copy next back to dp */
        for (int v = 0; v < 5; ++v)
            for (int f = 0; f < 2; ++f)
                dp[v][f] = next[v][f];
    }

    /* Final answer: sum over states where the last element's
       condition is satisfied (need==0). */
    seq_t ans = 0;
    for (int v = 0; v < 5; ++v)
        ans += dp[v][0];
    return ans;
}
