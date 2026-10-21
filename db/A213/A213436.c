/*
 * A213436  Principal diagonal of the convolution array A212891.
 * https://oeis.org/A213436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n^4 + 6*n^3 - 5*n^2)/12
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset is 1; return 0 for n==0 */
    seq_t sn  = (seq_t)n;
    seq_t sn2 = sn * sn;          /* n^2 */
    seq_t sn3 = sn2 * sn;         /* n^3 */
    seq_t sn4 = sn3 * sn;         /* n^4 */
    seq_t num = 11 * sn4 + 6 * sn3 - 5 * sn2;
    num /= 12;
    return num;
}
