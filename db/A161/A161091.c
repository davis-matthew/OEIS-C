/*
 * A161091  Number of partitions of n into squares where every part appears at
 *          least 3 times.
 * https://oeis.org/A161091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n into squares where every part appears
 *            at least 3 times
 * Range:     u8 n <= 122, u16 n <= 398, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#if defined(__cplusplus)
    #include <new>
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

/* Computes a(n) for n >= 1 (a(1)=0).  The function is re‑entrant and does not
   rely on any global state.  It runs in O(n * sqrt(n)) time with a modest
   memory footprint. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* undefined, but safe */

#if defined(__cplusplus)
    seq_t *dp   = new seq_t[n + 1]();       /* value‑initialises all elements */
    seq_t *newdp = new seq_t[n + 1]();
#else
    seq_t *dp   = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    seq_t *newdp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp || !newdp) {                    /* allocation failure */
        if (dp) free(dp);
        if (newdp) free(newdp);
        return 0;
    }
#endif

    dp[0] = 1;   /* one way to partition 0 (use no parts) */

    for (idx_t j = 1; ; ++j) {
        idx_t w = j * j;
        if (w > n) break;

        for (idx_t r = 0; r < w; ++r) {
            idx_t maxT = (n - r) / w;

            seq_t sumPrev = 0;      /* Σ_{k=0}^{t-1} dp[r + k*w] */
            seq_t last1   = 0;      /* dp[r + (t-1)*w]   */
            seq_t last2   = 0;      /* dp[r + (t-2)*w]   */

            for (idx_t t = 0; t <= maxT; ++t) {
                idx_t s = r + t * w;
                seq_t val_t = dp[s];
                seq_t result = val_t;

                if (t >= 3) {
                    /* partitions using at least 3 copies of w:
                       sum over all ways that leave at least 2 extra copies.
                       The kept part of the sum is prefix = sumPrev - last1 - last2
                       which corresponds to t-3, t-4, ... positions. */
                    seq_t prefix = sumPrev - last1 - last2;
                    result += prefix;
                }

                newdp[s] = result;

                /* shift the three longest suffixes for the next iteration */
                sumPrev += val_t;
                last2 = last1;
                last1 = val_t;
            }
        }

        /* swap dp and newdp for the next species of square */
#if defined(__cplusplus)
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
#else
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
#endif
    }

    seq_t answer = dp[n];

#if defined(__cplusplus)
    delete[] dp;
    delete[] newdp;
#else
    free(dp);
    free(newdp);
#endif

    return answer;
}
