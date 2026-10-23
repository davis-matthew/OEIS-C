/*
 * A169732  a(1) = 1000; for n>1, a(n) = a(n-1) - digitsum(a(n-1)).
 * https://oeis.org/A169732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1000; for n > 1, a(n) = a(n-1) - digit_sum(a(n-1))
 * Range:     u8 none, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so a(1) is the first term. */
    if (n == 0)
        return (seq_t)0;          /* undefined behaviour for n < 1, but harmless */
    if (n == 1)
        return (seq_t)1000;       /* a(1) = 1000 */

    seq_t val = (seq_t)1000;      /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        val -= digit_sum(val);
    }
    return val;
}
