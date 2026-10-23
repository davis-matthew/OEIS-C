/*
 * A165322  a(0)=1, a(1)=7, a(n)=15*a(n-1)-49*a(n-2) for n>1.
 * https://oeis.org/A165322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=7, a(n)=15*a(n-1)-49*a(n-2) for n>1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)7;

    seq_t prev2 = (seq_t)1;  /* a(0) */
    seq_t prev1 = (seq_t)7;  /* a(1) */
    seq_t curr = 0;

    for (idx_t i = 2; i <= n; ++i) {
        curr = prev1 * (seq_t)15 - prev2 * (seq_t)49;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
