/*
 * A016259  Expansion of 1/((1-x)(1-8x)(1-11x)).
 * https://oeis.org/A016259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = (7*11^(n+2) - 10*8^(n+2) + 3)/210
 * Recurrence: a(0)=1, a(1)=20, a(n)=19*a(n-1)-88*a(n-2)+1  (n>=2)
 */

#include <stdlib.h>   /* for size_t, malloc */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)20;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)20;  /* a(1) */
    seq_t cur   = 0;

    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)19 * prev1;
        cur -= (seq_t)88 * prev2;
        cur += (seq_t)1;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
