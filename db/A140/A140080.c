/*
 * A140080  Fix e = 3; a(n) = minimal Hamming distance between the binary
 *          representation of n and the binary representation of any multiple
 *          ke (0 <= k <= n/e) which is a child of n.
 * https://oeis.org/A140080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = popcount(n) - max_{k: 0 <= k <= n/3, (k*3) & ~n == 0}
 *            popcount(k*3)
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimal Hamming distance between n and any child multiple of 3 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Count bits of n (popcount) */
    int pop_n = __builtin_popcountll(n);

    /* DP over possible remainders modulo 3.
       dp[r] = maximal popcount of a submask of processed bits
               that yields remainder r when interpreted as the
               corresponding multiple of 3. */
    int dp[3] = {0, -1, -1};
    int newdp[3];

    /* Iterate over set bits of n */
    idx_t bits = n;
    int pos = 0;
    while (bits)
    {
        if (bits & 1ULL)
        {
            /* Weight of the current 1-bit in the multiple of 3 */
            int weight = (pos & 1) ? 2 : 1;   /* (2^pos) mod 3 */

            /* Start with ignoring this bit (exclude case) */
            for (int r = 0; r < 3; ++r)
                newdp[r] = dp[r];

            /* Include this bit in the submask */
            for (int r = 0; r < 3; ++r)
            {
                if (dp[r] >= 0)
                {
                    int rnew = (r + weight) % 3;
                    int val = dp[r] + 1;
                    if (val > newdp[rnew])
                        newdp[rnew] = val;
                }
            }

            /* Update dp for next iteration */
            for (int r = 0; r < 3; ++r)
                dp[r] = newdp[r];
        }

        bits >>= 1;
        ++pos;
    }

    /* The best submask is the one giving remainder 0 (divisible by 3) */
    int best = dp[0];
    /* Distance is total 1-bits minus those preserved in the best submask */
    int distance = pop_n - best;
    return (seq_t)distance;
}
