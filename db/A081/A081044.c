/*
 * A081044  9th binomial transform of (1,8,0,0,0,0,0,0,.....).
 * https://oeis.org/A081044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (8*n + 9) * 9^(n-1) * Recurrence: a(0) = 1, a(1) = 17,
 *            a(n) = 18*a(n-1) - 81*a(n-2) for n ≥ 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)17;

    /* Iterative computation – O(n), works for any order of calls */
    seq_t prev2 = (seq_t)1;   /* a(n-2) */
    seq_t prev1 = (seq_t)17;  /* a(n-1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = (seq_t)18 * prev1 - (seq_t)81 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
