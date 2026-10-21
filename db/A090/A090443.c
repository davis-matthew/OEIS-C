/*
 * A090443  a(n) = (n+2)! * (n+1)! * n! / 2.
 * https://oeis.org/A090443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) * n * (n+1) * (n+2), a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: a(n) = (n+2)! * (n+1)! * n! / 2  */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        res *= (seq_t)i;
        res *= (seq_t)(i + 1);
        res *= (seq_t)(i + 2);
    }
    return res;
}
