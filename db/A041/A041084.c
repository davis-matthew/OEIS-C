/*
 * A041084  Numerators of continued fraction convergents to sqrt(50).
 * https://oeis.org/A041084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 14*a(n-1)+a(n-2), a(0)=7, a(1)=99
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for standard integer types – does not define seq_t or idx_t */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)7;
    if (n == 1u)
        return (seq_t)99;

    seq_t prev2 = (seq_t)7;   /* a(0) */
    seq_t prev1 = (seq_t)99;  /* a(1) */
    seq_t cur = 0;

    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)14 * prev1 + prev2;  /* a(i) = 14*a(i-1)+a(i-2) */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
