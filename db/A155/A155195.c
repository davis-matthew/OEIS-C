/*
 * A155195  a(n)=6*a(n-1)+a(n-2), n>2 ; a(0)=1, a(1)=5, a(2)=30 .
 * https://oeis.org/A155195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + a(n-2), a(0) = 1, a(1) = 5, a(2) = 30
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)30;

    seq_t prev2 = (seq_t)30; /* a(n-1) */
    seq_t prev1 = (seq_t)5;  /* a(n-2) */
    seq_t next  = 0;

    for (idx_t i = 3; i <= n; ++i) {
        next  = prev2 * (seq_t)6 + prev1; /* a(i) */
        prev1 = prev2;
        prev2 = next;
    }
    return next;
}
