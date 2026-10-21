/*
 * A215415  a(2*n) = n, a(4*n+1) = 2*n-1, a(4*n+3) = 2*n+3.
 * https://oeis.org/A215415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(2*n)=n; a(4*n+1)=2*n-1; a(4*n+3)=2*n+3
 * Range:     i8 n <= 254, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Even indices */
    if ((n & 1) == 0)
        return (seq_t)(n >> 1);

    /* odd indices */
    if ((n & 3) == 1)          /* n ≡ 1 (mod 4)  →  a(n) = (n-3)/2  */
    {
        seq_t tn = (seq_t)n;
        return (tn - 3) / 2;
    }

    /* n ≡ 3 (mod 4)  →  a(n) = (n+3)/2 */
    seq_t tn = (seq_t)n;
    return (tn + 3) / 2;
}
