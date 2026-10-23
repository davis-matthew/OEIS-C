/*
 * A126418  Number of base 31 n-digit numbers with adjacent digits differing
 *          by two or less.
 * https://oeis.org/A126418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base‑31 n‑digit numbers with adjacent digits
 *            differing by at most 2. The sequence is computed using a simple
 *            DP: dp[i][d] = Σ dp[i‑1][k] for all k with |k‑d| ≤ 2. For i = 1
 *            each digit is reachable once: dp[1][d] = 1. a(n) = Σ dp[n][d]
 *            over d = 0..30
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* Number of digits in base‑31: 0 … 30 */
    const int BASE = 31;

    /* Empty string (n = 0) contributes 1 by convention. */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* DP rows: previous and current.  Local arrays are fine because
       the number of rows (BASE) is small.  For arbitrary‑precision
       seq_t we use element‑wise assignment instead of memcpy. */
    seq_t prev[BASE];
    seq_t curr[BASE];

    /* Initialise dp[1][d] = 1 for all digits. */
    for (int d = 0; d < BASE; ++d)
        prev[d] = (seq_t)1;

    /* Build DP for lengths 2 … n. */
    for (idx_t step = (idx_t)2; step <= n; ++step)
    {
        for (int d = 0; d < BASE; ++d)
        {
            int lower = d - 2;
            if (lower < 0) lower = 0;
            int upper = d + 2;
            if (upper >= BASE) upper = BASE - 1;

            seq_t sum = (seq_t)0;
            for (int k = lower; k <= upper; ++k)
                sum += prev[k];
            curr[d] = sum;
        }

        /* Move to the next length: copy curr into prev. */
        for (int i = 0; i < BASE; ++i)
            prev[i] = curr[i];
    }

    /* Sum all ending digits to obtain a(n). */
    seq_t total = (seq_t)0;
    for (int d = 0; d < BASE; ++d)
        total += prev[d];

    return total;
}
