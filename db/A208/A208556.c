/*
 * A208556  Number of 4 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A208556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^(n+1)+1)^2 for n even, a(n) = (2^(n+1)-1)^2 for n odd,
 *            n ≥ 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute 2^(n+1) */
    seq_t pow = pow2(n + 1);

    if (n & 1)          /* n is odd */
        return (pow - 1) * (pow - 1);
    else                /* n is even */
        return (pow + 1) * (pow + 1);
}
