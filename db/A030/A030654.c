/*
 * A030654  n^4*a(n) is the number of spheres in complex projective space
 *          tangent to 4 smooth surfaces of degree n in general position.
 * https://oeis.org/A030654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = n^8 + 4*n^6 - 2*n^4 + 4*n^2 + 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 255, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* values below the offset are undefined */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;            /* n^2 */
    seq_t n4 = n2 * n2;            /* n^4 */
    seq_t n6 = n4 * n2;            /* n^6 */
    seq_t n8 = n4 * n4;            /* n^8 */
    seq_t res = n8;
    res += 4 * n6;
    res -= 2 * n4;
    res += 4 * n2;
    res += 1;
    return res;
}
