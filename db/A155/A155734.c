/*
 * A155734  Binomial transform of A154879.
 * https://oeis.org/A155734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(0)=3; a(1)=1; a(n)=3*a(n-1) for n>1 (i.e. a(n)=3^(n-1) for
 *            n>=2)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0) return 3;
    if (n == 1) return 1;
    return pow3(n - 1);
}
