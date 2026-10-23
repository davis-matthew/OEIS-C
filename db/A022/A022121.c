/*
 * A022121  Fibonacci sequence beginning 3, 8.
 * https://oeis.org/A022121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 3, a(1) = 8
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t, used only in the typedefs */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == (idx_t)0)
        return (seq_t)3;
    if (n == (idx_t)1)
        return (seq_t)8;

    /* Iteratively compute the Fibonacci‑like recurrence. */
    seq_t prev2 = (seq_t)3;  /* a(0) */
    seq_t prev1 = (seq_t)8;  /* a(1) */
    seq_t cur;

    /* Loop from i = 2 up to and including n. */
    for (idx_t i = (idx_t)2; i <= n; ++i) {
        cur = prev1 + prev2;  /* a(i) = a(i-1) + a(i-2) */
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;  /* a(n) */
}
