/*
 * A022381  Fibonacci sequence beginning 3, 15.
 * https://oeis.org/A022381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-1) + a(n-2), a(0) = 3, a(1) = 15
 */
#include <stddef.h>   /* for size_t, if needed */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)15;

    /* Iterative computation using only two recent terms */
    seq_t prev2 = (seq_t)3;   /* a(n-2) */
    seq_t prev1 = (seq_t)15;  /* a(n-1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = prev2 + prev1;   /* a(i) = a(i-1)+a(i-2) */
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;   /* a(n) */
}
