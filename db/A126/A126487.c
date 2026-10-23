/*
 * A126487  Number of base 19 n-digit numbers with adjacent digits differing
 *          by three or less.
 * https://oeis.org/A126487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base‑19 n‑digit strings (leading zeros allowed)
 *            in which any two adjacent digits differ by at most 3. The code
 *            computes this via dynamic programming on the 19×19 transition
 *            matrix T[i][j] = 1 if |i-j| ≤ 3 else 0. It recomputes the counts
 *            from scratch for each call
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, NULL */
#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    /* Sequence index 0 gives the empty string. */
    if (n == 0)
        return (seq_t)1;

    /* Current counts for strings of the current length. */
    seq_t cur[19];
    for (int i = 0; i < 19; ++i)
        cur[i] = (seq_t)1;        /* length 1: one string for each digit */

    if (n == 1)
        return (seq_t)19;           /* sum of 19 ones */

    seq_t next[19];
    /* Iterate lengths 2..n */
    for (idx_t len = 2; len <= n; ++len)
    {
        for (int j = 0; j < 19; ++j)
        {
            int low = j - 3;
            if (low < 0) low = 0;
            int high = j + 3;
            if (high > 18) high = 18;
            seq_t sum = 0;
            for (int k = low; k <= high; ++k)
                sum += cur[k];
            next[j] = sum;
        }

        if (len == n)
        {
            /* Sum all strings of this length. */
            seq_t total = 0;
            for (int j = 0; j < 19; ++j)
                total += next[j];
            return total;
        }

        /* Prepare for next iteration. */
        for (int j = 0; j < 19; ++j)
            cur[j] = next[j];
    }

    /* Should never reach here. */
    return (seq_t)0;
}
