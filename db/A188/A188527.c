/*
 * A188527  a(n) = A188526(n)/7.
 * https://oeis.org/A188527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^(2*n + 3) + 2^n) / 7
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 4, a(1) = 35, a(n) = 11*a(n-1) - 18*a(n-2) for n >= 2 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)35;

    seq_t prev2 = (seq_t)4;   /* a(0) */
    seq_t prev1 = (seq_t)35;  /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur  = (seq_t)11 * prev1 - (seq_t)18 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
