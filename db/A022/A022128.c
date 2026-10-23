/*
 * A022128  Fibonacci sequence beginning 3, 19.
 * https://oeis.org/A022128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2), with a(0) = 3, a(1) = 19
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the first two elements directly. */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)19;

    /* Iteratively compute the sequence up to the requested index. */
    seq_t prevprev = (seq_t)3;   /* a(0) */
    seq_t prev     = (seq_t)19;  /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t curr = prev + prevprev;
        prevprev = prev;
        prev = curr;
    }
    return prev;
}
