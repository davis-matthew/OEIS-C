/*
 * A145186  Continued cotangent recurrence a(n+1)=a(n)^3+3*a(n) and a(1)=12.
 * https://oeis.org/A145186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = a(n)^3 + 3*a(n) with a(1) = 12
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, return 0 */
    seq_t val = (seq_t)12;                /* a(1) */
    if (n == 1) return val;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t tmp = val * val * val;      /* val^3 */
        tmp += (seq_t)3 * val;            /* +3*val */
        val = tmp;
    }
    return val;
}
