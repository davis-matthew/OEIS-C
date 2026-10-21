/*
 * A065874  a(n) = (7^(n+1) - (-6)^(n+1))/13.
 * https://oeis.org/A065874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 42*a(n-2), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t, if needed */

seq_t a(idx_t n)
{
    if (n <= 1)  /* a(0) and a(1) */
        return (seq_t)1;

    seq_t prev2 = (seq_t)1; /* a(0) */
    seq_t prev1 = (seq_t)1; /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur      = prev1 + (seq_t)42 * prev2;
        prev2    = prev1;
        prev1    = cur;
    }
    return prev1;            /* a(n) */
}
