/*
 * A165259  Sum of odd powers of 4 and 9 divided by 13.
 * https://oeis.org/A165259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 61, a(n) = 97*a(n-1) - 1296*a(n-2) for n >= 2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the two initial terms explicitly. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)61;

    /* Compute the recurrence iteratively.  Works for any idx_t (unsigned) and
       any seq_t (fixed‑width or arbitrary precision). */
    seq_t prev2 = (seq_t)1;          /* a(0) */
    seq_t prev1 = (seq_t)61;         /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = 97*a(i-1) - 1296*a(i-2) */
        cur = (seq_t)97 * prev1 - (seq_t)1296 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
