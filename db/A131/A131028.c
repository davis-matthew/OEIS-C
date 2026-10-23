/*
 * A131028  Periodic sequence (7, 4, 1, 1, 4, 7).
 * https://oeis.org/A131028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-6) for n>6; initial values: a(1)=7, a(2)=4, a(3)=1,
 *            a(4)=1, a(5)=4, a(6)=7
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t tbl[6] = {7, 7, 4, 1, 1, 4};
    if (n == 0) return 0; /* n>=1 by definition; return 0 for safety */
    return tbl[(size_t)(n % 6)];
}
