/*
 * A179207  a(n) = n - 1 + ceiling((-3 + n^2)/2) if n > 1 with a(1)=1,
 *          complement of A182835.
 * https://oeis.org/A179207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 2*n - 4)/2 for n>=2, a(1)=1
 * Range:     u8 n <= 21, u16 n <= 361, u32 n <= 9891, u64 n <= 9891,
 *            u128 n <= 9891, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    seq_t nn = (seq_t)n;
    /* compute (nn*nn + 2*nn - 4) / 2 */
    seq_t val = (nn * nn + nn * 2 - (seq_t)4) / (seq_t)2;
    return val;
}
