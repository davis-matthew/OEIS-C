/*
 * A024168  a(n) = n! * (1 + Sum_{j=1..n} (-1)^j/j).
 * https://oeis.org/A024168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2)*(n-1)^2, a(0)=1, a(1)=0
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    seq_t a0 = (seq_t)1; /* a(0) */
    seq_t a1 = (seq_t)0; /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t m  = (seq_t)(i - 1);       /* (i-1) */
        seq_t m2 = m * m;                /* (i-1)^2 */
        seq_t curr = a1 + a0 * m2;       /* a(i) = a(i-1) + a(i-2)*(i-1)^2 */
        a0 = a1;
        a1 = curr;
    }

    return a1;
}
