/*
 * A027872  a(n) = Product_{i=1..n} (5^i - 1).
 * https://oeis.org/A027872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n} (5^i - 1), with a(0)=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case: the offset is 0 */
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    seq_t pow5 = (seq_t)5;            /* 5^1 */

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t factor = pow5 - (seq_t)1;  /* 5^i - 1 */
        result *= factor;

        if (i < n)   /* prepare for next power of 5 */
            pow5 *= (seq_t)5;
    }

    return result;
}
