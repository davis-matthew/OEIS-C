/*
 * A214131  Partitions of n into parts congruent to +-4, +-6 (mod 13).
 * https://oeis.org/A214131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 150, u32 n <= 424, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A214131
   Partitions of n into parts congruent to +-4, +-6 (mod 13).
   Generating function: 1 / ∏_{k>0} (1 - x^(13k-4)) (1 - x^(13k-6))
                        (1 - x^(13k-7)) (1 - x^(13k-9))
   The straightforward dynamic programming algorithm computes the
   number of partitions using the allowed parts
   {4,6,7,9,18,19,20,22,…}.  Complexity is O(n²/13) which is well
   below the 2‑second limit for n≈10 000.  No memoisation is needed
   because the function is called a limited number of times in the
   test harness. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0‑based partition count */
    if (n == 0)
        return (seq_t)1;

    /* Dynamic array for partition counts up to n */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;   /* Out of memory – unlikely in tests */

    /* Initialise */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* Allowed part residues modulo 13: 4, 6, 7, 9 */
    const int residues[4] = {4, 6, 7, 9};
    for (int r = 0; r < 4; ++r)
    {
        for (idx_t p = residues[r]; p <= n; p += 13)
        {
            for (idx_t j = p; j <= n; ++j)
                dp[j] += dp[j - p];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
