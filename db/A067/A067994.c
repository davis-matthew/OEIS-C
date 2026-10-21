/*
 * A067994  Hermite numbers.
 * https://oeis.org/A067994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 0 if n is odd; a(0)=1; a(2k) = -(2*2k-2)*a(2k-2)
 *            (recurrence from a(n) = -(2*n-2)*a(n-2))
 * Range:     i8 n <= 7, i16 n <= 11, i32 n <= 17, i64 n <= 29, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for guaranteed integer width types */

seq_t a(idx_t n)
{
    /* odd indices give zero */
    if ((n & 1ULL) != 0)
        return 0;

    idx_t steps = n >> 1;           /* k = n/2 */
    seq_t result = 1;               /* a(0) = 1 */

    for (idx_t i = 1; i <= steps; ++i)
    {
        /* factor = -(4*i - 2) = -(2*(2*i)-2) = -(2*n-2) for n=2*i */
        seq_t factor = -(seq_t)(4 * i - 2);
        result *= factor;
    }

    return result;
}
