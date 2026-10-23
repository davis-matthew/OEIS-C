/*
 * A015005  q-factorial numbers for q=6.
 * https://oeis.org/A015005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = (6^n-1)*a(n-1)/5 for n ≥ 1 Equivalently a(n) =
 *            a(n-1) * ((6^n-1)/5)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;   /* a(0) */
    seq_t pow6   = 1;   /* 6^0 */
    for (idx_t i = 1; i <= n; ++i)
    {
        pow6 *= 6;                    /* 6^i */
        seq_t term = (pow6 - 1) / 5;  /* (6^i-1)/5 is integer */
        result *= term;               /* a(i) = a(i-1) * term */
    }
    return result;
}
