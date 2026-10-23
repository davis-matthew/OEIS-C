/*
 * A029199  Expansion of 1/((1-x^2)*(1-x^5)*(1-x^7)*(1-x^8)).
 * https://oeis.org/A029199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3+33*n^2+380*n+2595)/3360 - (n+16)*(n mod 2)/32
 *            + ((5*n^3+4*n^2+3*n+4) mod 7)/7)
 * Range:     u8 n <= 83, u16 n <= 592, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into parts 2,5,7,8 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate or grow a static table of dp values. */
    static seq_t *dp = NULL;
    static size_t cap = 0;
    size_t N = (size_t)n;          /* n is small (≤ 10000) for this sequence */

    /* If we don't have enough space, grow the array. */
    if (cap <= N) {
        size_t newcap = (cap == 0 ? 1 : cap);
        while (newcap <= N) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(dp, newcap * sizeof(seq_t));
        if (!tmp) exit(1);         /* allocation failure */
        /* Initialise the new entries to zero. */
        for (size_t i = cap; i < newcap; ++i) tmp[i] = 0;
        dp = tmp;
        cap = newcap;
    }

    /* Reset dp[0] … dp[N] to begin a new DP calculation. */
    dp[0] = 1;
    for (size_t i = 1; i <= N; ++i) dp[i] = 0;

    /* Parts allowed in this partition problem. */
    const idx_t parts[4] = {2, 5, 7, 8};

    /* Standard integer‑partition DP: for each part, accumulate ways
       to build each total. */
    for (int pi = 0; pi < 4; ++pi) {
        size_t part = (size_t)parts[pi];
        for (size_t i = part; i <= N; ++i) {
            dp[i] += dp[i - part];
        }
    }

    return dp[N];
}
