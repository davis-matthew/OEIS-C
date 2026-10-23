/*
 * A159221  1/2 the number of n X n arrays of squares of integers with every
 *          2X2 subblock summing to 20.
 * https://oeis.org/A159221
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 7 - 2^{floor(n/2)} (n ≥ 3, a(2)=9)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;            /* undefined for n < 2, offset starts at 2 */

    seq_t cur = (seq_t)9;           /* a(2) */
    if (n == 2)
        return cur;

    seq_t pow2 = (seq_t)2;          /* 2^{floor(2/2)} = 2^1 */

    for (idx_t i = 3; i <= n; ++i) {
        /* floor(i/2) increases when i is even (from odd to even) */
        if ((i & 1U) == 0)
            pow2 = pow2 * (seq_t)2;

        seq_t diff = (seq_t)7 - pow2;   /* 7 - 2^{floor(i/2)} */
        cur = cur * (seq_t)2 + diff;    /* a(i) = 2*a(i-1)+diff */
    }
    return cur;
}
