/*
 * A087216  a(n) = (6n)!/((3n)!(2n)!2^n).
 * https://oeis.org/A087216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (6n)! / ((3n)! (2n)! 2^n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(n)=a(n-1)*6*(6n-1)*(6n-5)/n for n>=1 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        /* compute multiplier = 6 * (6k-1) * (6k-5) as seq_t */
        idx_t sixk      = 6 * k;
        seq_t mult = (seq_t)6
                   * (seq_t)(sixk - 1)
                   * (seq_t)(sixk - 5);
        /* update result: result = result * mult / k */
        result = (result * mult) / (seq_t)k;
    }
    return result;
}
