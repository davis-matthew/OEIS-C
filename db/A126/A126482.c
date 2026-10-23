/*
 * A126482  Number of base 14 n-digit numbers with adjacent digits differing
 *          by three or less.
 * https://oeis.org/A126482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base‑14 n‑digit numbers whose adjacent digits
 *            differ by at most 3. Implemented by dynamic programming over the
 *            14 possible digits. a(0) = 1, which represents the empty string.
 *            For n≥1, each digit 0–13 is a state; transitions are allowed
 *            whenever |prev‑d| ≤ 3. Complexity: O(n) wit...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t if needed */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)14;

    seq_t curr[14];
    seq_t next[14];

    /* Base case: one‑digit numbers – each digit can start a number */
    for (int d = 0; d < 14; ++d)
        curr[d] = (seq_t)1;

    for (idx_t len = 2; len <= n; ++len) {
        for (int d = 0; d < 14; ++d) {
            seq_t sum = 0;
            int start = d - 3; if (start < 0) start = 0;
            int end   = d + 3; if (end   > 13) end = 13;
            for (int p = start; p <= end; ++p)
                sum += curr[p];
            next[d] = sum;
        }
        /* Prepare for next iteration */
        for (int d = 0; d < 14; ++d)
            curr[d] = next[d];
    }

    /* Sum over final digits to get total number of n‑digit strings */
    seq_t total = 0;
    for (int d = 0; d < 14; ++d)
        total += curr[d];
    return total;
}
